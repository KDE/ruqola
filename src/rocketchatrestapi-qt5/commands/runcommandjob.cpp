/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "runcommandjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
RunCommandJob::RunCommandJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RunCommandJob::~RunCommandJob() = default;

bool RunCommandJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RunCommandJob::start");
    submitPostRequest(json());

    return true;
}

void RunCommandJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RunCommandJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT runCommandDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("RunCommandJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

RunCommandJob::RunCommandInfo RunCommandJob::runCommandInfo() const
{
    return mRunCommandInfo;
}

void RunCommandJob::setRunCommandInfo(const RunCommandInfo &runCommandInfo)
{
    mRunCommandInfo = runCommandInfo;
}

RunCommandJob::RunCommandInfo RunCommandJob::parseString(const QString &str, const QString &roomId, const QString &tmid)
{
    RunCommandJob::RunCommandInfo info;
    if (str.length() > 1) {
        QString newStr = str.mid(1);
        QStringList lst = newStr.split(QLatin1Char(' '), Qt::SkipEmptyParts);
        const int numberElement = lst.count();
        info.commandName = lst.takeAt(0);
        info.roomId = roomId;
        info.threadMessageId = tmid;
        if (numberElement > 1) {
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

QDebug operator<<(QDebug d, const RunCommandJob::RunCommandInfo &t)
{
    d << " commandName " << t.commandName;
    d << " roomId " << t.roomId;
    d << " threadMessageId " << t.threadMessageId;
    d << " triggerId " << t.triggerId;
    d << " params " << t.params;
    return d;
}

#include "moc_runcommandjob.cpp"
