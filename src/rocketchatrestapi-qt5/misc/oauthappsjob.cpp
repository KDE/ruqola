/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
OauthAppsJob::OauthAppsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

OauthAppsJob::~OauthAppsJob() = default;

bool OauthAppsJob::requireHttpAuthentication() const
{
    return false;
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

void OauthAppsJob::onGetRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("OauthAppsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT oauthAppDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("OauthAppsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest OauthAppsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::OauthAppsList);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}
