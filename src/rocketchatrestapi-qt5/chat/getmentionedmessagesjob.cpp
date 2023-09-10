/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getmentionedmessagesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace RocketChatRestApi;
GetMentionedMessagesJob::GetMentionedMessagesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetMentionedMessagesJob::~GetMentionedMessagesJob() = default;

bool GetMentionedMessagesJob::requireHttpAuthentication() const
{
    return true;
}

bool GetMentionedMessagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "getMentionedMessagesJob: mRoomId is empty";
        return false;
    }
    return true;
}

bool GetMentionedMessagesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getMentionedMessagesJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("getMentionedMessagesJob: Ask starred messages"));
    return true;
}

void GetMentionedMessagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("getMentionedMessagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getMentionedMessagesDone(replyObject, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("getMentionedMessagesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString GetMentionedMessagesJob::roomId() const
{
    return mRoomId;
}

void GetMentionedMessagesJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest GetMentionedMessagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatGetMentionedMessages);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), mRoomId);
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request, false);

    addAuthRawHeader(request);
    return request;
}

bool GetMentionedMessagesJob::hasQueryParameterSupport() const
{
    return true;
}

#include "moc_getmentionedmessagesjob.cpp"
