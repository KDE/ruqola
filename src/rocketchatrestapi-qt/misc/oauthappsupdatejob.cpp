/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappsupdatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
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
    if (replyObject[QLatin1StringView("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("OauthAppsUpdateJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT oauthAppsUpdateDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("OauthAppsUpdateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj[QLatin1StringView("name")] = mOauthAppsUpdateInfo.name;
    jsonObj[QLatin1StringView("redirectUri")] = mOauthAppsUpdateInfo.redirectUri;
    jsonObj[QLatin1StringView("active")] = mOauthAppsUpdateInfo.active;
    jsonObj[QLatin1StringView("appId")] = mOauthAppsUpdateInfo.appId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool OauthAppsUpdateJob::OauthAppsUpdateInfo::isValid() const
{
    return !redirectUri.isEmpty() && !name.isEmpty() && !appId.isEmpty();
}

#include "moc_oauthappsupdatejob.cpp"
