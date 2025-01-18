/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appupdateinfojob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
AppUpdateInfoJob::AppUpdateInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

AppUpdateInfoJob::~AppUpdateInfoJob() = default;

bool AppUpdateInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool AppUpdateInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start get app count job";
        deleteLater();
        return false;
    }
    switch (mAppMode) {
    case AppMode::Unknown:
        break;
    case AppMode::Delete:
        submitDeleteRequest();
        addStartRestApiInfo(QByteArrayLiteral("AppUpdateInfoJob: delete starting"));
        break;
    case AppMode::Get:
        submitGetRequest();
        addStartRestApiInfo(QByteArrayLiteral("AppUpdateInfoJob: get starting"));
        break;
    case AppMode::Post:
        submitPostRequest(json());
        addStartRestApiInfo(QByteArrayLiteral("AppUpdateInfoJob: post starting"));
        break;
    }

    return true;
}

bool AppUpdateInfoJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mAppInfoType == AppInfoType::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mAppInfoType undefined";
        return false;
    }
    if (mAppMode == AppMode::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mAppMode undefined";
        return false;
    }
    if (mAppsId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mAppsId is empty";
        return false;
    }
    return true;
}

QJsonDocument AppUpdateInfoJob::json() const
{
    // TODO
    QJsonObject jsonObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

AppUpdateInfoJob::AppInfoType AppUpdateInfoJob::appInfoType() const
{
    return mAppInfoType;
}

void AppUpdateInfoJob::setAppInfoType(AppInfoType newAppInfoType)
{
    mAppInfoType = newAppInfoType;
}

QByteArray AppUpdateInfoJob::appsId() const
{
    return mAppsId;
}

void AppUpdateInfoJob::setAppsId(const QByteArray &newAppsId)
{
    mAppsId = newAppsId;
}

QString AppUpdateInfoJob::generateUrlExtension() const
{
    QString url = QString::fromLatin1(mAppsId);

    switch (mAppInfoType) {
    case AppInfoType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Unknown type";
        break;
    case AppInfoType::Apps:
        break;
    case AppInfoType::Settings:
        url += QLatin1Char('/') + QStringLiteral("settings");
        break;
    case AppInfoType::Status:
        url += QLatin1Char('/') + QStringLiteral("status");
        break;
    }
    return url;
}

QString AppUpdateInfoJob::appVersion() const
{
    return mAppVersion;
}

void AppUpdateInfoJob::setAppVersion(const QString &newAppVersion)
{
    mAppVersion = newAppVersion;
}

AppUpdateInfoJob::AppMode AppUpdateInfoJob::appMode() const
{
    return mAppMode;
}

void AppUpdateInfoJob::setAppMode(AppMode newAppMode)
{
    mAppMode = newAppMode;
}

QNetworkRequest AppUpdateInfoJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Empty, RestApiUtil::RestApiUrlExtensionType::Apps, generateUrlExtension());
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

void AppUpdateInfoJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("AppUpdateInfoJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT appUpdateInfoDone(replyObject["data"_L1].toObject());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("AppUpdateInfoJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

#include "moc_appupdateinfojob.cpp"
