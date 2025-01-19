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
    switch (mAppUpdateInfo.mAppMode) {
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
    if (!mAppUpdateInfo.isValid()) {
        return false;
    }
    return true;
}

QJsonDocument AppUpdateInfoJob::json() const
{
    QJsonObject jsonObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    if (mAppUpdateInfo.mAppInfoType == AppInfoType::Status) {
        jsonObj["status"_L1] = "manually_disabled"_L1;
    }
    // TODO status {"status":"manually_enabled"} or {"status":"manually_disabled"}
    return postData;
}

QString AppUpdateInfoJob::generateUrlExtension() const
{
    QString url = QString::fromLatin1(mAppUpdateInfo.mAppsId);

    switch (mAppUpdateInfo.mAppInfoType) {
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

AppUpdateInfoJob::AppUpdateInfo AppUpdateInfoJob::appUpdateInfo() const
{
    return mAppUpdateInfo;
}

void AppUpdateInfoJob::setAppUpdateInfo(const AppUpdateInfo &newAppUpdateInfo)
{
    mAppUpdateInfo = newAppUpdateInfo;
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

bool AppUpdateInfoJob::AppUpdateInfo::isValid() const
{
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

#include "moc_appupdateinfojob.cpp"
