/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "videoconfupdatejitsitimeoutjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
VideoConfUpdateJitsiTimeOutJob::VideoConfUpdateJitsiTimeOutJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

VideoConfUpdateJitsiTimeOutJob::~VideoConfUpdateJitsiTimeOutJob()
{
}

bool VideoConfUpdateJitsiTimeOutJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("VideoConfUpdateJitsiTimeOutJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &VideoConfUpdateJitsiTimeOutJob::slotSetAvatar);
    return true;
}

void VideoConfUpdateJitsiTimeOutJob::slotSetAvatar()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        addLoggerInfo(QByteArrayLiteral("VideoConfUpdateJitsiTimeOutJob: finished: ") + data);
        Q_EMIT setAvatarDone();
    }
    deleteLater();
}

QString VideoConfUpdateJitsiTimeOutJob::avatarUrl() const
{
    return mAvatarUrl;
}

void VideoConfUpdateJitsiTimeOutJob::setAvatarUrl(const QString &avatarUrl)
{
    mAvatarUrl = avatarUrl;
}

QString VideoConfUpdateJitsiTimeOutJob::avatarUserId() const
{
    return mAvatarUserId;
}

void VideoConfUpdateJitsiTimeOutJob::setAvatarUserId(const QString &avatarUserId)
{
    mAvatarUserId = avatarUserId;
}

bool VideoConfUpdateJitsiTimeOutJob::requireHttpAuthentication() const
{
    return true;
}

bool VideoConfUpdateJitsiTimeOutJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start VideoConfUpdateJitsiTimeOutJob";
        return false;
    }
    if (mAvatarUrl.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "VideoConfUpdateJitsiTimeOutJob: mAvatarUrl is empty";
        return false;
    }
    return true;
}

QNetworkRequest VideoConfUpdateJitsiTimeOutJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersSetAvatar);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument VideoConfUpdateJitsiTimeOutJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("avatarUrl")] = mAvatarUrl;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
