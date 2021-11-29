/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

SetUserActiveStatusJob::~SetUserActiveStatusJob() = default;

bool SetUserActiveStatusJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SetUserActiveStatusJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &SetUserActiveStatusJob::slotSetUserActiveStatus);
    return true;
}

void SetUserActiveStatusJob::slotSetUserActiveStatus()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SetUserActiveStatusJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT setUserActiveStatusDone(replyObject);
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
