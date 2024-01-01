/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT UpdateAdminSettingsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct UpdateAdminSettingsInfo {
        enum ValueType {
            Unknown,
            Boolean,
            Integer,
            String,
        };

        QString settingName;
        QVariant settingsValue;
        UpdateAdminSettingsInfo::ValueType valueType = UpdateAdminSettingsInfo::ValueType::Unknown;
        [[nodiscard]] bool isValid() const;
        [[nodiscard]] bool canStart() const;
    };
    explicit UpdateAdminSettingsJob(QObject *parent = nullptr);
    ~UpdateAdminSettingsJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;
    [[nodiscard]] const UpdateAdminSettingsInfo &updateAdminSettingsInfo() const;
    void setUpdateAdminSettingsInfo(const UpdateAdminSettingsInfo &newInfo);

    [[nodiscard]] bool requireTwoFactorAuthentication() const override;

Q_SIGNALS:
    void updateAdminSettingsDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(UpdateAdminSettingsJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    UpdateAdminSettingsInfo mInfo;
};
}
