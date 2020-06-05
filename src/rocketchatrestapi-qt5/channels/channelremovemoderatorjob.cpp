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

#include "channelremovemoderatorjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelRemoveModeratorJob::ChannelRemoveModeratorJob(QObject *parent)
    : ChannelBaseJob(parent)
{
}

ChannelRemoveModeratorJob::~ChannelRemoveModeratorJob()
{
}

bool ChannelRemoveModeratorJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelRemoveModeratorJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &ChannelRemoveModeratorJob::slotRemoveModeratorFinished);
    return true;
}

void ChannelRemoveModeratorJob::slotRemoveModeratorFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChannelRemoveModeratorJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT removeModeratorDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ChannelRemoveModeratorJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString ChannelRemoveModeratorJob::removeUserId() const
{
    return mRemoveUserId;
}

void ChannelRemoveModeratorJob::setRemoveUserId(const QString &removeUserId)
{
    mRemoveUserId = removeUserId;
}

bool ChannelRemoveModeratorJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelRemoveModeratorJob::canStart() const
{
    if (mRemoveUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelRemoveModeratorJob: remove userid is empty";
        return false;
    }
    if (!hasRoomIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelRemoveModeratorJob: RoomId and RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChannelRemoveModeratorJob::json() const
{
    QJsonObject jsonObj;
    generateJSon(jsonObj);
    jsonObj[QLatin1String("userId")] = removeUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChannelRemoveModeratorJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsRemoveModerator);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
