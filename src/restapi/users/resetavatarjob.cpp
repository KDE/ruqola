/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "resetavatarjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

ResetAvatarJob::ResetAvatarJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ResetAvatarJob::~ResetAvatarJob()
{
}

bool ResetAvatarJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ResetAvatarJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ResetAvatarJob::slotResetAvatar);
    return true;
}

void ResetAvatarJob::slotResetAvatar()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        addLoggerInfo(QByteArrayLiteral("ResetAvatarJob: finished: ") + data);
        Q_EMIT resetAvatarDone();
    }
    deleteLater();
}

QString ResetAvatarJob::avatarUserId() const
{
    return mAvatarUserId;
}

void ResetAvatarJob::setAvatarUserId(const QString &avatarUserId)
{
    mAvatarUserId = avatarUserId;
}

bool ResetAvatarJob::requireHttpAuthentication() const
{
    return true;
}

bool ResetAvatarJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start ResetAvatarJob";
        return false;
    }
    if (mAvatarUserId.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "ResetAvatarJob: mAvatarUserId is empty";
        return false;
    }
    return true;
}

QNetworkRequest ResetAvatarJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersResetAvatar);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}

QJsonDocument ResetAvatarJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("userId")] = mAvatarUserId;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
