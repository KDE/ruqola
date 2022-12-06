/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappscreatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
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

void OauthAppsCreateJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("OauthAppsCreateJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT oauthAppsCreateDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("OauthAppsCreateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
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
    //    if (mPermissions.isEmpty()) {
    //        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "OauthAppsCreateJob: mPermissions is empty";
    //        return false;
    //    }
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
    // TODO
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
