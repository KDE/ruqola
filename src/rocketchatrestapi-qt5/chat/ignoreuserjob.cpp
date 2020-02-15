/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "ignoreuserjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
IgnoreUserJob::IgnoreUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

IgnoreUserJob::~IgnoreUserJob()
{
}

bool IgnoreUserJob::requireHttpAuthentication() const
{
    return true;
}

bool IgnoreUserJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ignore user job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &IgnoreUserJob::slotIgnoreUserFinished);
    addLoggerInfo(QByteArrayLiteral("IgnoreUserJob: ignore user starting"));
    return true;
}

void IgnoreUserJob::slotIgnoreUserFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("IgnoreUserJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT ignoreUserDone(replyObject, mRoomId);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("Problem when we tried to ignore user message: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool IgnoreUserJob::ignore() const
{
    return mIgnore;
}

void IgnoreUserJob::setIgnore(bool ignore)
{
    mIgnore = ignore;
}

QString IgnoreUserJob::ignoreUserId() const
{
    return mIgnoreUserId;
}

void IgnoreUserJob::setIgnoreUserId(const QString &ignoreUserId)
{
    mIgnoreUserId = ignoreUserId;
}

QString IgnoreUserJob::roomId() const
{
    return mRoomId;
}

void IgnoreUserJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest IgnoreUserJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatIgnoreUser);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("rid"), mRoomId);
    queryUrl.addQueryItem(QStringLiteral("userId"), mIgnoreUserId);
    queryUrl.addQueryItem(QStringLiteral("ignore"), mIgnore ? QStringLiteral("true") : QStringLiteral("false"));
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
#else
    request.setAttribute(QNetworkRequest::Http2AllowedAttribute, true);
#endif
    return request;
}

bool IgnoreUserJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ignore user job";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomId is empty";
        return false;
    }
    if (mIgnoreUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ignore user id is empty";
        return false;
    }
    return true;
}
