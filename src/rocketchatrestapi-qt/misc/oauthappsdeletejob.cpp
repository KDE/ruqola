/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappsdeletejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
OauthAppsDeleteJob::OauthAppsDeleteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

OauthAppsDeleteJob::~OauthAppsDeleteJob() = default;

bool OauthAppsDeleteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("OauthAppsDeleteJob::start");
    submitPostRequest(json());

    return true;
}

void OauthAppsDeleteJob::onPostRequestResponse([[maybe_unused]] const QString &replyErrorString, const QJsonDocument &replyJson)
{
    // const QJsonObject replyObject = replyJson.object();
    // No result from RC server ? bug from RC server
    Q_EMIT oauthAppsDeleteDone();

    // qDebug() << " DDDDDDDDDDDDDDDDDD " << replyJson;
    // if (replyObject["success"_L1].toBool()) {
    //     addLoggerInfo("OauthAppsDeleteJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
    //     Q_EMIT oauthAppsDeleteDone(replyObject);
    // } else {
    //     emitFailedMessage(replyErrorString, replyObject);
    //     addLoggerWarning("OauthAppsDeleteJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    // }
}

QString OauthAppsDeleteJob::identifier() const
{
    return mIdentifier;
}

void OauthAppsDeleteJob::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

bool OauthAppsDeleteJob::requireHttpAuthentication() const
{
    return true;
}

bool OauthAppsDeleteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mIdentifier.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "OauthAppsDeleteJob: mIdentifier is empty";
        return false;
    }
    return true;
}

QNetworkRequest OauthAppsDeleteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::OauthAppsDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument OauthAppsDeleteJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["appId"_L1] = mIdentifier;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    qDebug() << " postData " << postData;
    return postData;
}

#include "moc_oauthappsdeletejob.cpp"
