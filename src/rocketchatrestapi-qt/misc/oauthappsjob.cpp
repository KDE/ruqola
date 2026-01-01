/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappsjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace RocketChatRestApi;
OauthAppsJob::OauthAppsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

bool OauthAppsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mAppId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "OauthAppsJob: mAppId is undefined";
        return false;
    }
    if (mClientId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "OauthAppsJob: mClientId is undefined";
        return false;
    }
    return true;
}

OauthAppsJob::~OauthAppsJob() = default;

bool OauthAppsJob::requireHttpAuthentication() const
{
    return true;
}

bool OauthAppsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start OauthAppsJob";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("OauthAppsJob: Ask settings oauth"_ba);
    return true;
}

void OauthAppsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("OauthAppsJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT oauthAppDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("OauthAppsJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QString &OauthAppsJob::clientId() const
{
    return mClientId;
}

void OauthAppsJob::setClientId(const QString &newClientId)
{
    mClientId = newClientId;
}

const QString &OauthAppsJob::appId() const
{
    return mAppId;
}

void OauthAppsJob::setAppId(const QString &newAppId)
{
    mAppId = newAppId;
}

QNetworkRequest OauthAppsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::OauthAppsGet);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"clientId"_s, mClientId);
    queryUrl.addQueryItem(u"appId"_s, mAppId);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_oauthappsjob.cpp"
