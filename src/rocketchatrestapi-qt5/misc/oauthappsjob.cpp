/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
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

    addStartRestApiInfo(QByteArrayLiteral("OauthAppsJob: Ask settings oauth"));
    return true;
}

void OauthAppsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("OauthAppsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT oauthAppDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("OauthAppsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
    queryUrl.addQueryItem(QStringLiteral("clientId"), mClientId);
    queryUrl.addQueryItem(QStringLiteral("appId"), mAppId);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_oauthappsjob.cpp"
