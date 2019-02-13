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

#include "groupremoveownerjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
GroupRemoveOwnerJob::GroupRemoveOwnerJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GroupRemoveOwnerJob::~GroupRemoveOwnerJob()
{
}

bool GroupRemoveOwnerJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("GroupRemoveOwnerJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &GroupRemoveOwnerJob::slotRemoveOwnerFinished);
    return true;
}

void GroupRemoveOwnerJob::slotRemoveOwnerFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GroupRemoveOwnerJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT removeOwnerDone();
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("GroupRemoveOwnerJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
            if (replyObject[QStringLiteral("errorType")].toString() == QLatin1String("error-remove-last-owner")) {
                Q_EMIT failed(i18n("This is the last owner. Please set a new owner before removing this one."));
            }
        }
    }
    deleteLater();
}

QString GroupRemoveOwnerJob::removeUserId() const
{
    return mRemoveUserId;
}

void GroupRemoveOwnerJob::setRemoveUserId(const QString &removeUserId)
{
    mRemoveUserId = removeUserId;
}

bool GroupRemoveOwnerJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupRemoveOwnerJob::canStart() const
{
    if (mRemoveUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupRemoveOwnerJob: remove userid is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupRemoveOwnerJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GroupRemoveOwnerJob job";
        return false;
    }
    return true;
}

QJsonDocument GroupRemoveOwnerJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("userId")] = removeUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString GroupRemoveOwnerJob::roomId() const
{
    return mRoomId;
}

void GroupRemoveOwnerJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest GroupRemoveOwnerJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupRemoveOwner);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
