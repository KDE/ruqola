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

#include "changechanneldescriptionjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
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
    addStartRestApiInfo("ChangeChannelDescriptionJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &ChangeChannelDescriptionJob::slotChangeDescriptionFinished);
    return true;
}

void ChangeChannelDescriptionJob::slotChangeDescriptionFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("Change description success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT changeDescriptionDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("Problem when we tried to change description: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
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
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeChannelDescriptionJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
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
    addRequestAttribute(request);
    return request;
}
