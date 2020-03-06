/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "runcommandjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
RunCommandJob::RunCommandJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RunCommandJob::~RunCommandJob()
{
}

bool RunCommandJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("RunCommandJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &RunCommandJob::slotRunCommand);
    return true;
}

void RunCommandJob::slotRunCommand()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RunCommandJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT runCommandDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RunCommandJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

RunCommandJob::RunCommandInfo RunCommandJob::runCommandInfo() const
{
    return mRunCommandInfo;
}

void RunCommandJob::setRunCommandInfo(const RunCommandInfo &runCommandInfo)
{
    mRunCommandInfo = runCommandInfo;
}

RunCommandJob::RunCommandInfo RunCommandJob::parseString(const QString &str, const QString &roomId)
{
    RunCommandJob::RunCommandInfo info;
    if (str.length() > 1) {
        QString newStr = str.mid(1);
        QStringList lst = newStr.split(QLatin1Char(' '), Qt::SkipEmptyParts);
        const int numberElement = lst.count();
        info.commandName = lst.takeAt(0);
        info.roomId = roomId;
        if (numberElement == 2) {
            info.params = lst.join(QLatin1Char(' '));
        }
    }
    return info;
}

bool RunCommandJob::requireHttpAuthentication() const
{
    return true;
}

bool RunCommandJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mRunCommandInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RunCommandJob: RoomId and CommandName are empty";
        return false;
    }
    return true;
}

QNetworkRequest RunCommandJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CommandsRun);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RunCommandJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("command")] = mRunCommandInfo.commandName;
    jsonObj[QLatin1String("roomId")] = mRunCommandInfo.roomId;
    if (!mRunCommandInfo.threadMessageId.isEmpty()) {
        jsonObj[QLatin1String("tmid")] = mRunCommandInfo.threadMessageId;
    }
    if (!mRunCommandInfo.triggerId.isEmpty()) {
        jsonObj[QLatin1String("triggerId")] = mRunCommandInfo.triggerId;
    }

    if (!mRunCommandInfo.params.isEmpty()) {
        jsonObj[QLatin1String("params")] = mRunCommandInfo.params;
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool RunCommandJob::RunCommandInfo::isValid() const
{
    return !commandName.isEmpty() && !roomId.isEmpty();
}
