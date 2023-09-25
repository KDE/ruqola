/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserreportedmessagesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
ModerationUserReportedMessagesJob::ModerationUserReportedMessagesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ModerationUserReportedMessagesJob::~ModerationUserReportedMessagesJob() = default;

bool ModerationUserReportedMessagesJob::requireHttpAuthentication() const
{
    return true;
}

bool ModerationUserReportedMessagesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start get message job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("ModerationReportJob: get message starting"));
    return true;
}

void ModerationUserReportedMessagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ModerationReportJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationUserReportedMessagesDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ModerationReportJob: Problem when we tried to get message : ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ModerationUserReportedMessagesJob::reportedMessageFromUserId() const
{
    return mReportedMessageFromUserId;
}

void ModerationUserReportedMessagesJob::setReportedMessageFromUserId(const QString &newReportedMessageFromUserId)
{
    mReportedMessageFromUserId = newReportedMessageFromUserId;
}

QNetworkRequest ModerationUserReportedMessagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ModerationUserReportedMessages);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("userId"), mReportedMessageFromUserId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool ModerationUserReportedMessagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mReportedMessageFromUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "MessageId is empty";
        return false;
    }
    return true;
}

#include "moc_moderationuserreportedmessagesjob.cpp"
