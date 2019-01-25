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
    connect(reply, &QNetworkReply::finished, this, &VideoConfUpdateJitsiTimeOutJob::slotUpdateJitsiTimeOut);
    return true;
}

void VideoConfUpdateJitsiTimeOutJob::slotUpdateJitsiTimeOut()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("VideoConfUpdateJitsiTimeOutJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT updateJitsiTimeOutDone(replyObject[QStringLiteral("jitsiTimeout")].toString());
        } else {
            addLoggerWarning(QByteArrayLiteral("VideoConfUpdateJitsiTimeOutJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
    }
    deleteLater();
}

QString VideoConfUpdateJitsiTimeOutJob::roomId() const
{
    return mRoomId;
}

void VideoConfUpdateJitsiTimeOutJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
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
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "VideoConfUpdateJitsiTimeOutJob: mRoomid is empty";
        return false;
    }
    return true;
}

QNetworkRequest VideoConfUpdateJitsiTimeOutJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::VideoConfJitsi);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument VideoConfUpdateJitsiTimeOutJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mRoomId;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
