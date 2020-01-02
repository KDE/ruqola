/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

GetStarredMessagesJob::~GetStarredMessagesJob()
{
}

bool GetStarredMessagesJob::requireHttpAuthentication() const
{
    return true;
}

bool GetStarredMessagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetStarredMessagesJob";
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
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &GetStarredMessagesJob::slotGetStarredMessagesFinished);
    addLoggerInfo(QByteArrayLiteral("GetStarredMessagesJob: Ask starred messages"));
    return true;
}

void GetStarredMessagesJob::slotGetStarredMessagesFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetStarredMessagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT getStarredMessagesDone(replyObject, mRoomId);
        } else {
            emitFailedMessage(replyObject);
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
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    addAuthRawHeader(request);
    return request;
}

bool GetStarredMessagesJob::hasQueryParameterSupport() const
{
    return true;
}
