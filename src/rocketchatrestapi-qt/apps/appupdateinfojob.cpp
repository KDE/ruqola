/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appupdateinfojob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>

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
        addStartRestApiInfo("AppUpdateInfoJob: delete starting"_ba);
        break;
    case AppMode::Get:
        submitGetRequest();
        addStartRestApiInfo("AppUpdateInfoJob: get starting"_ba);
        break;
    case AppMode::Post:
        submitPostRequest(json());
        addStartRestApiInfo("AppUpdateInfoJob: post starting"_ba);
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
    if (mAppUpdateInfo.mAppInfoType == AppInfoType::Status) {
        jsonObj["status"_L1] = mAppUpdateInfo.mStatus;
    } else if (mAppUpdateInfo.mAppMode == AppMode::Post && mAppUpdateInfo.mAppInfoType == AppInfoType::Apps) {
        jsonObj["appId"_L1] = QString::fromLatin1(mAppUpdateInfo.mAppsId);
        jsonObj["version"_L1] = mAppUpdateInfo.mAppVersion;
        jsonObj["marketplace"_L1] = true;
    } else if (mAppUpdateInfo.mAppInfoType == AppInfoType::Settings) {
        jsonObj["settings"_L1] = mAppUpdateInfo.mAppsSettings;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    // qDebug() << " postData: " << postData;
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
        // In post mode we need only https://<url>/api/apps
        if (mAppUpdateInfo.mAppMode == AppMode::Post) {
            return {};
        }
        break;
    case AppInfoType::Settings:
        url += u'/' + u"settings"_s;
        break;
    case AppInfoType::Status:
        url += u'/' + u"status"_s;
        break;
    case AppInfoType::ActionButton:
        url = u"actionButtons"_s;
        break;
    case AppInfoType::Icon:
        url += u'/' + u"icon"_s;
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
        addLoggerInfo("AppUpdateInfoJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT appUpdateInfoDone(replyObject["data"_L1].toObject());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("AppUpdateInfoJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

void AppUpdateInfoJob::onDeleteRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("Delete AppUpdateInfoJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT appUpdateInfoDone(replyObject["data"_L1].toObject());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("Delete AppUpdateInfoJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

void AppUpdateInfoJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (mAppUpdateInfo.mAppInfoType == AppInfoType::ActionButton) {
        Q_EMIT fetchActionButtonsDone(replyJson.array());
    } else {
        const QJsonObject replyObject = replyJson.object();
        if (replyObject["status"_L1].toString() == "success"_L1) {
            addLoggerInfo("Get: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
            qCDebug(ROCKETCHATQTRESTAPI_LOG) << " Logout";
            Q_EMIT appUpdateInfoDone(replyObject["data"_L1].toObject());
        } else {
            emitFailedMessage(replyErrorString, replyObject);
            addLoggerWarning("AppUpdateInfoJob: Error" + replyJson.toJson(QJsonDocument::Indented));
        }
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
    if (mAppMode == AppMode::Get && mAppInfoType == AppInfoType::ActionButton) {
        return true;
    }
    if (mAppsId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mAppsId is empty";
        return false;
    }
    return true;
}

#include "moc_appupdateinfojob.cpp"
