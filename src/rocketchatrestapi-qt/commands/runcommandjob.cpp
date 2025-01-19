/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "runcommandjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
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

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RunCommandJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT runCommandDone();
    } else {
        Q_EMIT runCommandFailed(mRunCommandInfo);
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

RunCommandJob::RunCommandInfo RunCommandJob::parseString(const QString &str, const QByteArray &roomId, const QByteArray &tmid)
{
    RunCommandJob::RunCommandInfo info;
    if (str.length() > 1) {
        const QString newStr = str.mid(1);
        QStringList lst = newStr.split(QLatin1Char(' '), Qt::SkipEmptyParts);
        const int numberElement = lst.count();
        info.commandName = lst.takeAt(0);
        info.roomId = QLatin1StringView(roomId);
        info.threadMessageId = QLatin1StringView(tmid);
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
    jsonObj["command"_L1] = mRunCommandInfo.commandName;
    jsonObj["roomId"_L1] = mRunCommandInfo.roomId;
    if (!mRunCommandInfo.threadMessageId.isEmpty()) {
        jsonObj["tmid"_L1] = mRunCommandInfo.threadMessageId;
    }
    if (!mRunCommandInfo.triggerId.isEmpty()) {
        jsonObj["triggerId"_L1] = mRunCommandInfo.triggerId;
    }

    if (!mRunCommandInfo.params.isEmpty()) {
        jsonObj["params"_L1] = mRunCommandInfo.params;
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
    d.space() << "commandName" << t.commandName;
    d.space() << "roomId" << t.roomId;
    d.space() << "threadMessageId" << t.threadMessageId;
    d.space() << "triggerId" << t.triggerId;
    d.space() << "params" << t.params;
    return d;
}

#include "moc_runcommandjob.cpp"
