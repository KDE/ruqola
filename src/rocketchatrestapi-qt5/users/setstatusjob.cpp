/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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

SetStatusJob::~SetStatusJob() = default;

bool SetStatusJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SetStatusJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &SetStatusJob::slotSetStatus);
    return true;
}

void SetStatusJob::slotSetStatus()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SetStatusJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT setStatusDone();
        } else {
            emitFailedMessage(replyObject, reply);
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
