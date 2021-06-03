/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

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

#include "setuseractivestatusjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
SetUserActiveStatusJob::SetUserActiveStatusJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SetUserActiveStatusJob::~SetUserActiveStatusJob()
{
}

bool SetUserActiveStatusJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SetUserActiveStatusJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &SetUserActiveStatusJob::slotSetStatus);
    return true;
}

void SetUserActiveStatusJob::slotSetStatus()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SetUserActiveStatusJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT setStatusDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("SetUserActiveStatusJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

const QString &SetUserActiveStatusJob::activateUserId() const
{
    return mActivateUserId;
}

void SetUserActiveStatusJob::setActivateUserId(const QString &newActivateUserId)
{
    mActivateUserId = newActivateUserId;
}

bool SetUserActiveStatusJob::activate() const
{
    return mActivate;
}

void SetUserActiveStatusJob::setActivate(bool newActivate)
{
    mActivate = newActivate;
}

bool SetUserActiveStatusJob::requireHttpAuthentication() const
{
    return true;
}

bool SetUserActiveStatusJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mActivateUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetUserActiveStatusJob: mUserId is empty";
        return false;
    }
    return true;
}

QNetworkRequest SetUserActiveStatusJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersSetActiveStatus);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SetUserActiveStatusJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("userId")] = mActivateUserId;
    jsonObj[QLatin1String("activeStatus")] = mActivate;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
