/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getstarredmessagesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace RocketChatRestApi;
GetStarredMessagesJob::GetStarredMessagesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetStarredMessagesJob::~GetStarredMessagesJob() = default;

bool GetStarredMessagesJob::requireHttpAuthentication() const
{
    return true;
}

bool GetStarredMessagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetStarredMessagesJob: mRoomId is empty";
        return false;
    }
    return true;
}

bool GetStarredMessagesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetStarredMessagesJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &GetStarredMessagesJob::slotGetStarredMessagesFinished);
    addStartRestApiInfo(QByteArrayLiteral("GetStarredMessagesJob: Ask starred messages"));
    return true;
}

void GetStarredMessagesJob::slotGetStarredMessagesFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetStarredMessagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT getStarredMessagesDone(replyObject, mRoomId);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("GetStarredMessagesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString GetStarredMessagesJob::roomId() const
{
    return mRoomId;
}

void GetStarredMessagesJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest GetStarredMessagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatGetStarredMessages);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), mRoomId);
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request, false);

    addAuthRawHeader(request);
    return request;
}

bool GetStarredMessagesJob::hasQueryParameterSupport() const
{
    return true;
}
