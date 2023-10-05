/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserdeletereportedmessagesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ModerationUserDeleteReportedMessagesJob::ModerationUserDeleteReportedMessagesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ModerationUserDeleteReportedMessagesJob::~ModerationUserDeleteReportedMessagesJob() = default;

bool ModerationUserDeleteReportedMessagesJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ModerationUserDeleteReportedMessagesJob::start");
    submitPostRequest(json());

    return true;
}

void ModerationUserDeleteReportedMessagesJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ModerationUserDeleteReportedMessagesJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationUserDeleteReportedMessagesDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ModerationUserDeleteReportedMessagesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ModerationUserDeleteReportedMessagesJob::userIdForMessages() const
{
    return mUserIdForMessages;
}

void ModerationUserDeleteReportedMessagesJob::setUserIdForMessages(const QString &newUserIdForMessages)
{
    mUserIdForMessages = newUserIdForMessages;
}

bool ModerationUserDeleteReportedMessagesJob::requireHttpAuthentication() const
{
    return true;
}

bool ModerationUserDeleteReportedMessagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mUserIdForMessages.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ModerationUserDeleteReportedMessagesJob: mUserIdForMessages is empty";
        return false;
    }
    return true;
}

QNetworkRequest ModerationUserDeleteReportedMessagesJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ModerationUserDeleteReportedMessages);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ModerationUserDeleteReportedMessagesJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("userId")] = mUserIdForMessages;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_moderationuserdeletereportedmessagesjob.cpp"
