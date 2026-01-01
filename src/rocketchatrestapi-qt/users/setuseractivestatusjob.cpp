/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setuseractivestatusjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
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
    submitPostRequest(json());
    return true;
}

void SetUserActiveStatusJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SetUserActiveStatusJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT setUserActiveStatusDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SetUserActiveStatusJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QByteArray &SetUserActiveStatusJob::activateUserId() const
{
    return mActivateUserId;
}

void SetUserActiveStatusJob::setActivateUserId(const QByteArray &newActivateUserId)
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
    jsonObj["userId"_L1] = QLatin1StringView(mActivateUserId);
    jsonObj["activeStatus"_L1] = mActivate;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_setuseractivestatusjob.cpp"
