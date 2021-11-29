/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

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
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &GetMentionedMessagesJob::slotGetMentionedMessagesFinished);
    addStartRestApiInfo(QByteArrayLiteral("getMentionedMessagesJob: Ask starred messages"));
    return true;
}

void GetMentionedMessagesJob::slotGetMentionedMessagesFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("getMentionedMessagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT getMentionedMessagesDone(replyObject, mRoomId);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("getMentionedMessagesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
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
