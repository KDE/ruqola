/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationdismissreportsjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

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

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ModerationDismissReportsJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationDismissReportsDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ModerationDismissReportsJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray ModerationDismissReportsJob::messageId() const
{
    return mMessageId;
}

void ModerationDismissReportsJob::setMessageId(const QByteArray &newMessageId)
{
    mMessageId = newMessageId;
}

QByteArray ModerationDismissReportsJob::userIdForMessages() const
{
    return mUserIdForMessages;
}

void ModerationDismissReportsJob::setUserIdForMessages(const QByteArray &newUserIdForMessages)
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
    if (!mUserIdForMessages.isEmpty()) {
        jsonObj["userId"_L1] = QLatin1StringView(mUserIdForMessages);
    } else if (!mMessageId.isEmpty()) {
        jsonObj["msgId"_L1] = QLatin1StringView(mMessageId);
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_moderationdismissreportsjob.cpp"
