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

#include "setstatusjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
SetStatusJob::SetStatusJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SetStatusJob::~SetStatusJob()
{
}

bool SetStatusJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("SetStatusJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &SetStatusJob::slotSetStatus);
    return true;
}

void SetStatusJob::slotSetStatus()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SetStatusJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT setStatusDone();
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("SetStatusJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString SetStatusJob::statusUserId() const
{
    return mStatusUserId;
}

void SetStatusJob::setStatusUserId(const QString &statusUserId)
{
    mStatusUserId = statusUserId;
}

SetStatusJob::StatusType SetStatusJob::status() const
{
    return mStatus;
}

void SetStatusJob::setStatus(StatusType status)
{
    mStatus = status;
}

QString SetStatusJob::statusMessage() const
{
    return mStatusMessage;
}

void SetStatusJob::setStatusMessage(const QString &statusMessage)
{
    mStatusMessage = statusMessage;
}

bool SetStatusJob::requireHttpAuthentication() const
{
    return true;
}

bool SetStatusJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SetStatusJob";
        return false;
    }
    if (mStatusUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetStatusJob: mUserId is empty";
        return false;
    }
    if (mStatus == SetStatusJob::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetStatusJob: mStatus is not defined";
        return false;
    }
    return true;
}

QNetworkRequest SetStatusJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersSetStatus);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SetStatusJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("userId")] = mStatusUserId;
    QString statusType;
    switch (mStatus) {
    case OnLine:
        statusType = QStringLiteral("online");
        break;
    case Away:
        statusType = QStringLiteral("away");
        break;
    case Offline:
        statusType = QStringLiteral("offline");
        break;
    case Busy:
        statusType = QStringLiteral("busy");
        break;
    case Unknown:
        break;
    }
    jsonObj[QLatin1String("status")] = statusType;
    jsonObj[QLatin1String("message")] = mStatusMessage;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
