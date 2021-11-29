/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "getsnippetedmessagesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace RocketChatRestApi;
GetSnippetedMessagesJob::GetSnippetedMessagesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetSnippetedMessagesJob::~GetSnippetedMessagesJob() = default;

bool GetSnippetedMessagesJob::requireHttpAuthentication() const
{
    return true;
}

bool GetSnippetedMessagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetSnippetedMessagesJob: mRoomId is empty";
        return false;
    }
    return true;
}

bool GetSnippetedMessagesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetSnippetedMessagesJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &GetSnippetedMessagesJob::slotGetSnippetedMessagesFinished);
    addStartRestApiInfo(QByteArrayLiteral("GetSnippetedMessagesJob: Ask starred messages"));
    return true;
}

void GetSnippetedMessagesJob::slotGetSnippetedMessagesFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetSnippetedMessagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT getSnippetedMessagesDone(replyObject, mRoomId);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("GetSnippetedMessagesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString GetSnippetedMessagesJob::roomId() const
{
    return mRoomId;
}

void GetSnippetedMessagesJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest GetSnippetedMessagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatGetSnippetedMessages);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), mRoomId);
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request, false);

    addAuthRawHeader(request);
    return request;
}

bool GetSnippetedMessagesJob::hasQueryParameterSupport() const
{
    return true;
}
