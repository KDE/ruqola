/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getmentionedmessagesjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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

    addStartRestApiInfo("getMentionedMessagesJob: Ask starred messages"_ba);
    return true;
}

void GetMentionedMessagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("getMentionedMessagesJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getMentionedMessagesDone(replyObject, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("getMentionedMessagesJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray GetMentionedMessagesJob::roomId() const
{
    return mRoomId;
}

void GetMentionedMessagesJob::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest GetMentionedMessagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatGetMentionedMessages);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"roomId"_s, QLatin1StringView(mRoomId));
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request);

    addAuthRawHeader(request);
    return request;
}

bool GetMentionedMessagesJob::hasQueryParameterSupport() const
{
    return true;
}

#include "moc_getmentionedmessagesjob.cpp"
