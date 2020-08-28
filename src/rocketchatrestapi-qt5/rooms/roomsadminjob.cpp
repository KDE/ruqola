/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "roomsadminjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
RoomsAdminJob::RoomsAdminJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsAdminJob::~RoomsAdminJob()
{
}

bool RoomsAdminJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsAdminJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start roomAdmin job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &RoomsAdminJob::slotRoomsAdminFinished);
    addStartRestApiInfo(QByteArrayLiteral("RoomsAdminJob: Ask info about room admin info"));
    return true;
}

void RoomsAdminJob::slotRoomsAdminFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RoomsAdminJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT roomsAdminDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RoomsAdminJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

RoomsAdminJob::RoomsAdminJobInfo RoomsAdminJob::roomsAdminInfo() const
{
    return mRoomsAdminInfo;
}

void RoomsAdminJob::setRoomsAdminInfo(const RoomsAdminJobInfo &roomsAdminInfo)
{
    mRoomsAdminInfo = roomsAdminInfo;
}

QNetworkRequest RoomsAdminJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsAdminRooms);
    if (mRoomsAdminInfo.isValid()) {
        QUrlQuery queryUrl;
        queryUrl.addQueryItem(QStringLiteral("filter"), mRoomsAdminInfo.filter);
        addQueryParameter(queryUrl);
        url.setQuery(queryUrl);
    }

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

bool RoomsAdminJob::RoomsAdminJobInfo::isValid() const
{
    return !filter.isEmpty();
}
