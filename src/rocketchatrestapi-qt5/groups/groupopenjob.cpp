/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "groupopenjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

using namespace RocketChatRestApi;
GroupOpenJob::GroupOpenJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GroupOpenJob::~GroupOpenJob()
{
}

bool GroupOpenJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("GroupOpenJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &GroupOpenJob::slotChannelOpenFinished);
    return true;
}

void GroupOpenJob::slotChannelOpenFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GroupOpenJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT channelOpenDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("GroupOpenJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool GroupOpenJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupOpenJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    //    if (!hasRoomIdentifier()) {
    //        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupOpenJob: RoomId and RoomName are empty";
    //        return false;
    //    }
    return true;
}

QJsonDocument GroupOpenJob::json() const
{
    QJsonObject jsonObj;
    // generateJson(jsonObj);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest GroupOpenJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsOpen);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
