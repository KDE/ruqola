/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappscreatejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
OauthAppsCreateJob::OauthAppsCreateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

OauthAppsCreateJob::~OauthAppsCreateJob() = default;

bool OauthAppsCreateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("OauthAppsCreateJob::start");
    submitPostRequest(json());

    return true;
}

void OauthAppsCreateJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("OauthAppsCreateJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT oauthAppsCreateDone(replyObject["application"_L1].toObject());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("OauthAppsCreateJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

OauthAppsCreateJob::OauthAppsCreateInfo OauthAppsCreateJob::oauthAppsCreateInfo() const
{
    return mOauthAppsCreateInfo;
}

void OauthAppsCreateJob::setOauthAppsCreateInfo(const OauthAppsCreateInfo &newOauthAppsCreateInfo)
{
    mOauthAppsCreateInfo = newOauthAppsCreateInfo;
}

bool OauthAppsCreateJob::requireHttpAuthentication() const
{
    return true;
}

bool OauthAppsCreateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mOauthAppsCreateInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "OauthAppsCreateJob: mOauthAppsCreateInfo is invalid";
        return false;
    }
    return true;
}

QNetworkRequest OauthAppsCreateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::OauthAppsCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument OauthAppsCreateJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["name"_L1] = mOauthAppsCreateInfo.name;
    jsonObj["redirectUri"_L1] = mOauthAppsCreateInfo.redirectUri;
    jsonObj["active"_L1] = mOauthAppsCreateInfo.active;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool OauthAppsCreateJob::OauthAppsCreateInfo::isValid() const
{
    return !redirectUri.isEmpty() && !name.isEmpty();
}

#include "moc_oauthappscreatejob.cpp"
