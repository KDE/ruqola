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

#include "changechannelnamejob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChangeChannelNameJob::ChangeChannelNameJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeChannelNameJob::~ChangeChannelNameJob()
{
}

bool ChangeChannelNameJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ChangeChannelNameJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ChangeChannelNameJob::slotChangeNameFinished);
    return true;
}

void ChangeChannelNameJob::slotChangeNameFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("Change name success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT changeNameDone();
        } else {
            addLoggerWarning(QByteArrayLiteral("Problem when we tried to change name: ") + replyJson.toJson(QJsonDocument::Indented));
        }
    }
    deleteLater();
}

bool ChangeChannelNameJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeChannelNameJob::canStart() const
{
    if (mName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeChannelNameJob: name is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeChannelNameJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChangeChannelNameJob job";
        return false;
    }
    return true;
}

QJsonDocument ChangeChannelNameJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("name")] = name();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeChannelNameJob::roomId() const
{
    return mRoomId;
}

void ChangeChannelNameJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString ChangeChannelNameJob::name() const
{
    return mName;
}

void ChangeChannelNameJob::setName(const QString &t)
{
    mName = t;
}

QNetworkRequest ChangeChannelNameJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsRename);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
