/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappsupdatejob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
OauthAppsUpdateJob::OauthAppsUpdateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

OauthAppsUpdateJob::~OauthAppsUpdateJob() = default;

bool OauthAppsUpdateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("OauthAppsUpdateJob::start");
    submitPostRequest(json());

    return true;
}

void OauthAppsUpdateJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("OauthAppsUpdateJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT oauthAppsUpdateDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("OauthAppsUpdateJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

OauthAppsUpdateJob::OauthAppsUpdateInfo OauthAppsUpdateJob::oauthAppsUpdateInfo() const
{
    return mOauthAppsUpdateInfo;
}

void OauthAppsUpdateJob::setOauthAppsUpdateInfo(const OauthAppsUpdateInfo &newOauthAppsUpdateInfo)
{
    mOauthAppsUpdateInfo = newOauthAppsUpdateInfo;
}

bool OauthAppsUpdateJob::requireHttpAuthentication() const
{
    return true;
}

bool OauthAppsUpdateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mOauthAppsUpdateInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "OauthAppsUpdateJob: mOauthAppsUpdateInfo is invalid";
        return false;
    }
    return true;
}

QNetworkRequest OauthAppsUpdateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::OauthAppsUpdate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument OauthAppsUpdateJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["name"_L1] = mOauthAppsUpdateInfo.name;
    jsonObj["redirectUri"_L1] = mOauthAppsUpdateInfo.redirectUri;
    jsonObj["active"_L1] = mOauthAppsUpdateInfo.active;
    jsonObj["appId"_L1] = mOauthAppsUpdateInfo.appId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool OauthAppsUpdateJob::OauthAppsUpdateInfo::isValid() const
{
    return !redirectUri.isEmpty() && !name.isEmpty() && !appId.isEmpty();
}

#include "moc_oauthappsupdatejob.cpp"
