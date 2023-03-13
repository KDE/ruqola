/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "updateadminsettingsjob.h"
#include "restapimethod.h"

#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UpdateAdminSettingsJob::UpdateAdminSettingsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UpdateAdminSettingsJob::~UpdateAdminSettingsJob() = default;

bool UpdateAdminSettingsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UpdateAdminSettingsJob::start");
    submitPostRequest(json());
    return true;
}

void UpdateAdminSettingsJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UpdateAdminSettingsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT updateAdminSettingsDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("UpdateAdminSettingsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

const UpdateAdminSettingsJob::UpdateAdminSettingsInfo &UpdateAdminSettingsJob::updateAdminSettingsInfo() const
{
    return mInfo;
}

void UpdateAdminSettingsJob::setUpdateAdminSettingsInfo(const UpdateAdminSettingsInfo &newInfo)
{
    mInfo = newInfo;
}

bool UpdateAdminSettingsJob::requireHttpAuthentication() const
{
    return true;
}

bool UpdateAdminSettingsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mInfo.canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest UpdateAdminSettingsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UpdateAdminSettings, mInfo.settingName);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UpdateAdminSettingsJob::json() const
{
    QJsonObject jsonObj;
    switch (mInfo.valueType) {
    case UpdateAdminSettingsInfo::ValueType::Unknown:
        break;
    case UpdateAdminSettingsInfo::ValueType::Boolean:
        jsonObj[QLatin1String("value")] = mInfo.settingsValue.toBool();
        break;
    case UpdateAdminSettingsInfo::ValueType::Integer:
        jsonObj[QLatin1String("value")] = mInfo.settingsValue.toInt();
        break;
    case UpdateAdminSettingsInfo::ValueType::String:
        jsonObj[QLatin1String("value")] = mInfo.settingsValue.toString();
        break;
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool UpdateAdminSettingsJob::UpdateAdminSettingsInfo::isValid() const
{
    return !settingName.isEmpty() && valueType != UpdateAdminSettingsInfo::ValueType::Unknown;
}

bool UpdateAdminSettingsJob::UpdateAdminSettingsInfo::canStart() const
{
    return isValid();
}

bool UpdateAdminSettingsJob::requireTwoFactorAuthentication() const
{
    return true;
}
