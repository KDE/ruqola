/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getpinnedmessagesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace RocketChatRestApi;
GetPinnedMessagesJob::GetPinnedMessagesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetPinnedMessagesJob::~GetPinnedMessagesJob() = default;

bool GetPinnedMessagesJob::requireHttpAuthentication() const
{
    return true;
}

bool GetPinnedMessagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetPinnedMessagesJob: mRoomId is empty";
        return false;
    }
    return true;
}

bool GetPinnedMessagesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetPinnedMessagesJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("GetPinnedMessagesJob: Ask pinned messages"));
    return true;
}

void GetPinnedMessagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GetPinnedMessagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getPinnedMessagesDone(replyObject, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("GetPinnedMessagesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString GetPinnedMessagesJob::roomId() const
{
    return mRoomId;
}

void GetPinnedMessagesJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest GetPinnedMessagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatGetPinnedMessages);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), mRoomId);
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request, false);

    addAuthRawHeader(request);
    return request;
}

bool GetPinnedMessagesJob::hasQueryParameterSupport() const
{
    return true;
}
