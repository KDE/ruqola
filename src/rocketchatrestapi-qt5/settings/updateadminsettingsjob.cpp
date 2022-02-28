/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "updateadminsettingsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

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

void UpdateAdminSettingsJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UpdateAdminSettingsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT updateAdminSettingsDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("UpdateAdminSettingsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

const UpdateAdminSettingsJob::UpdateAdminSettingsInfo &UpdateAdminSettingsJob::info() const
{
    return mInfo;
}

void UpdateAdminSettingsJob::setInfo(const UpdateAdminSettingsInfo &newInfo)
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
    // TODO
    return {};
}

bool UpdateAdminSettingsJob::UpdateAdminSettingsInfo::isValid() const
{
    return !settingName.isEmpty();
}

bool UpdateAdminSettingsJob::UpdateAdminSettingsInfo::canStart() const
{
    return isValid();
}
