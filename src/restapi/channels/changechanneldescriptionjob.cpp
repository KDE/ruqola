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

#include "changechanneldescriptionjob.h"

#include "ruqola_restapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

ChangeChannelDescriptionJob::ChangeChannelDescriptionJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeChannelDescriptionJob::~ChangeChannelDescriptionJob()
{
}

bool ChangeChannelDescriptionJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ChangeChannelDescriptionJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ChangeChannelDescriptionJob::slotChangeDescriptionFinished);
    return true;
}

void ChangeChannelDescriptionJob::slotChangeDescriptionFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(RUQOLA_RESTAPI_LOG) << "Change description success: " << data;
            Q_EMIT changeDescriptionDone();
        } else {
            qCWarning(RUQOLA_RESTAPI_LOG) <<" Problem when we tried to change description: " << data;
        }
    }
    deleteLater();
}

QString ChangeChannelDescriptionJob::description() const
{
    return mDescription;
}

void ChangeChannelDescriptionJob::setDescription(const QString &description)
{
    mDescription = description;
}

bool ChangeChannelDescriptionJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeChannelDescriptionJob::canStart() const
{
    if (mDescription.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "ChangeChannelDescriptionJob: description is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "ChangeChannelDescriptionJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start ChangeChannelDescriptionJob job";
        return false;
    }
    return true;
}

QJsonDocument ChangeChannelDescriptionJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("description")] = description();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeChannelDescriptionJob::roomId() const
{
    return mRoomId;
}

void ChangeChannelDescriptionJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ChangeChannelDescriptionJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetDescription);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
