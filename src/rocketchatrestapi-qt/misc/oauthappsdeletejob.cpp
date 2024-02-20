/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappsdeletejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
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

void OauthAppsDeleteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    // No result from RC server ? bug from RC server
    Q_EMIT oauthAppsDeleteDone();

    // qDebug() << " DDDDDDDDDDDDDDDDDD " << replyJson;
    // if (replyObject[QLatin1String("success")].toBool()) {
    //     addLoggerInfo(QByteArrayLiteral("OauthAppsDeleteJob success: ") + replyJson.toJson(QJsonDocument::Indented));
    //     Q_EMIT oauthAppsDeleteDone(replyObject);
    // } else {
    //     emitFailedMessage(replyErrorString, replyObject);
    //     addLoggerWarning(QByteArrayLiteral("OauthAppsDeleteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj[QLatin1String("appId")] = mIdentifier;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    qDebug() << " postData " << postData;
    return postData;
}

#include "moc_oauthappsdeletejob.cpp"
