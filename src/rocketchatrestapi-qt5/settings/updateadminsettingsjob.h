/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT UpdateAdminSettingsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct UpdateAdminSettingsInfo {
        QString settingName;
        QVariant settingsValue;
        Q_REQUIRED_RESULT bool isValid() const;
        Q_REQUIRED_RESULT bool canStart() const;
    };
    explicit UpdateAdminSettingsJob(QObject *parent = nullptr);
    ~UpdateAdminSettingsJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;
    Q_REQUIRED_RESULT const UpdateAdminSettingsInfo &updateAdminSettingsInfo() const;
    void setUpdateAdminSettingsInfo(const UpdateAdminSettingsInfo &newInfo);

    Q_REQUIRED_RESULT bool requireTwoFactorAuthentication() const override;

Q_SIGNALS:
    void updateAdminSettingsDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(UpdateAdminSettingsJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    UpdateAdminSettingsInfo mInfo;
};
}
