/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
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
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ResetAvatarJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT resetAvatarDone();
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("ResetAvatarJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
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
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ResetAvatarJob";
        return false;
    }
    if (mAvatarUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ResetAvatarJob: mAvatarUserId is empty";
        return false;
    }
    return true;
}

QNetworkRequest ResetAvatarJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersResetAvatar);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ResetAvatarJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("userId")] = mAvatarUserId;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
