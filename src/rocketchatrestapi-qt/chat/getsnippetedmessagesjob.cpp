/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getsnippetedmessagesjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;
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

    addStartRestApiInfo("GetSnippetedMessagesJob: Ask starred messages"_ba);
    return true;
}

void GetSnippetedMessagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GetSnippetedMessagesJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getSnippetedMessagesDone(replyObject, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GetSnippetedMessagesJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray GetSnippetedMessagesJob::roomId() const
{
    return mRoomId;
}

void GetSnippetedMessagesJob::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest GetSnippetedMessagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatGetSnippetedMessages);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"roomId"_s, QLatin1StringView(mRoomId));
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request);

    addAuthRawHeader(request);
    return request;
}

bool GetSnippetedMessagesJob::hasQueryParameterSupport() const
{
    return true;
}

#include "moc_getsnippetedmessagesjob.cpp"
