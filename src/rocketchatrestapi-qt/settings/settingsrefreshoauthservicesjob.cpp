/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settingsrefreshoauthservicesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SettingsRefreshOAuthServicesJob::SettingsRefreshOAuthServicesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SettingsRefreshOAuthServicesJob::~SettingsRefreshOAuthServicesJob() = default;

bool SettingsRefreshOAuthServicesJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SettingsRefreshOAuthServicesJob::start"_ba);
    submitPostRequest(json());
    return true;
}

void SettingsRefreshOAuthServicesJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("SettingsRefreshOAuthServicesJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT updateAdminSettingsDone(*replyObject);
    }
}

const SettingsRefreshOAuthServicesJob::UpdateAdminSettingsInfo &SettingsRefreshOAuthServicesJob::updateAdminSettingsInfo() const
{
    return mInfo;
}

void SettingsRefreshOAuthServicesJob::setUpdateAdminSettingsInfo(const UpdateAdminSettingsInfo &newInfo)
{
    mInfo = newInfo;
}

bool SettingsRefreshOAuthServicesJob::requireHttpAuthentication() const
{
    return true;
}

bool SettingsRefreshOAuthServicesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mInfo.canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest SettingsRefreshOAuthServicesJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UpdateAdminSettings, RestApiUtil::RestApiUrlExtensionType::V1, mInfo.settingName);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SettingsRefreshOAuthServicesJob::json() const
{
    QJsonObject jsonObj;
    switch (mInfo.valueType) {
    case UpdateAdminSettingsInfo::ValueType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << " Value type is unknown. It's a bug";
        break;
    case UpdateAdminSettingsInfo::ValueType::Boolean:
        jsonObj["value"_L1] = mInfo.settingsValue.toBool();
        break;
    case UpdateAdminSettingsInfo::ValueType::Integer:
        jsonObj["value"_L1] = mInfo.settingsValue.toInt();
        break;
    case UpdateAdminSettingsInfo::ValueType::String:
        jsonObj["value"_L1] = mInfo.settingsValue.toString();
        break;
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool SettingsRefreshOAuthServicesJob::UpdateAdminSettingsInfo::isValid() const
{
    return !settingName.isEmpty() && valueType != UpdateAdminSettingsInfo::ValueType::Unknown;
}

bool SettingsRefreshOAuthServicesJob::UpdateAdminSettingsInfo::canStart() const
{
    return isValid();
}

bool SettingsRefreshOAuthServicesJob::requireTwoFactorAuthentication() const
{
    return true;
}

#include "moc_settingsrefreshoauthservicesjob.cpp"
