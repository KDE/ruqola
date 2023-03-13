/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("GetSnippetedMessagesJob: Ask starred messages"));
    return true;
}

void GetSnippetedMessagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GetSnippetedMessagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getSnippetedMessagesDone(replyObject, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("GetSnippetedMessagesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
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
