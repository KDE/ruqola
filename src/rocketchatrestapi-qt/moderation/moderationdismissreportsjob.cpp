/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationdismissreportsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ModerationDismissReportsJob::ModerationDismissReportsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ModerationDismissReportsJob::~ModerationDismissReportsJob() = default;

bool ModerationDismissReportsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ModerationDismissReportsJob::start");
    submitPostRequest(json());

    return true;
}

void ModerationDismissReportsJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ModerationDismissReportsJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationDismissReportsDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ModerationDismissReportsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ModerationDismissReportsJob::messageId() const
{
    return mMessageId;
}

void ModerationDismissReportsJob::setMessageId(const QString &newMessageId)
{
    mMessageId = newMessageId;
}

QString ModerationDismissReportsJob::userIdForMessages() const
{
    return mUserIdForMessages;
}

void ModerationDismissReportsJob::setUserIdForMessages(const QString &newUserIdForMessages)
{
    mUserIdForMessages = newUserIdForMessages;
}

bool ModerationDismissReportsJob::requireHttpAuthentication() const
{
    return true;
}

bool ModerationDismissReportsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mUserIdForMessages.isEmpty() && mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ModerationDismissReportsJob: mUserIdForMessages is empty and mMessageId isEmpty()";
        return false;
    }
    return true;
}

QNetworkRequest ModerationDismissReportsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ModerationDismissReports);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ModerationDismissReportsJob::json() const
{
    QJsonObject jsonObj;
    if (mUserIdForMessages.isEmpty()) {
        jsonObj[QLatin1String("userId")] = mUserIdForMessages;
    } else if (mMessageId.isEmpty()) {
        jsonObj[QLatin1String("msgId")] = mMessageId;
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_moderationdismissreportsjob.cpp"
