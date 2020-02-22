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

#include "leavechanneljob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
LeaveChannelJob::LeaveChannelJob(QObject *parent)
    : ChannelBaseJob(parent)
{
}

LeaveChannelJob::~LeaveChannelJob()
{
}

bool LeaveChannelJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("LeaveChannelJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &LeaveChannelJob::slotLeaveChannelFinished);
    return true;
}

void LeaveChannelJob::slotLeaveChannelFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("LeaveChannelJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT leaveChannelDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("LeaveChannelJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool LeaveChannelJob::requireHttpAuthentication() const
{
    return true;
}

bool LeaveChannelJob::canStart() const
{
    if (!hasRoomIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "LeaveChannelJob: RoomId and RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument LeaveChannelJob::json() const
{
    QJsonObject jsonObj;
    generateJSon(jsonObj);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest LeaveChannelJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsLeave);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
