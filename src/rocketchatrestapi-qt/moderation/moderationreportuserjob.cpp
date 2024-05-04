/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportuserjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
ModerationReportUserJob::ModerationReportUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ModerationReportUserJob::~ModerationReportUserJob() = default;

bool ModerationReportUserJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ModerationReportUserJob::start");
    submitPostRequest(json());

    return true;
}

void ModerationReportUserJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ModerationReportUserJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationDismissReportsDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ModerationReportUserJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray ModerationReportUserJob::messageId() const
{
    return mMessageId;
}

void ModerationReportUserJob::setMessageId(const QByteArray &newMessageId)
{
    mMessageId = newMessageId;
}

QByteArray ModerationReportUserJob::userIdForMessages() const
{
    return mUserIdForMessages;
}

void ModerationReportUserJob::setUserIdForMessages(const QByteArray &newUserIdForMessages)
{
    mUserIdForMessages = newUserIdForMessages;
}

bool ModerationReportUserJob::requireHttpAuthentication() const
{
    return true;
}

bool ModerationReportUserJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mUserIdForMessages.isEmpty() && mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ModerationReportUserJob: mUserIdForMessages is empty and mMessageId isEmpty()";
        return false;
    }
    return true;
}

QNetworkRequest ModerationReportUserJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ModerationDismissReports);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ModerationReportUserJob::json() const
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

#include "moc_moderationreportuserjob.cpp"
