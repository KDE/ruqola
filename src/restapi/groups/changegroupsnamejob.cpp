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

#include "changegroupsnamejob.h"

#include "restapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

ChangeGroupsNameJob::ChangeGroupsNameJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeGroupsNameJob::~ChangeGroupsNameJob()
{
}

bool ChangeGroupsNameJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ChangeGroupsNameJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ChangeGroupsNameJob::slotChangeGroupsNameFinished);
    return true;
}

void ChangeGroupsNameJob::slotChangeGroupsNameFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(RESTAPI_LOG) << "Change name success: " << data;
            Q_EMIT changeGroupsnameDone();
        } else {
            qCWarning(RESTAPI_LOG) <<" Problem when we tried to change name" << data;
        }
    }
    deleteLater();
}

bool ChangeGroupsNameJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeGroupsNameJob::canStart() const
{
    if (mName.isEmpty()) {
        qCWarning(RESTAPI_LOG) << "ChangeGroupsNameJob: name is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(RESTAPI_LOG) << "ChangeGroupsNameJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RESTAPI_LOG) << "Impossible to start ChangeGroupsNameJob job";
        return false;
    }
    return true;
}

QJsonDocument ChangeGroupsNameJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("name")] = name();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeGroupsNameJob::roomId() const
{
    return mRoomId;
}

void ChangeGroupsNameJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString ChangeGroupsNameJob::name() const
{
    return mName;
}

void ChangeGroupsNameJob::setName(const QString &t)
{
    mName = t;
}

QNetworkRequest ChangeGroupsNameJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsRename);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
