/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT AppUpdateInfoJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    enum class AppMode : uint8_t {
        Unknown,
        Post,
        Get,
        Delete,
    };

    enum class AppInfoType : uint8_t {
        Unknown,
        Settings,
        Apps,
        Status,
    };

    struct AppUpdateInfo {
        QByteArray mAppsId;
        QString mAppVersion;
        AppInfoType mAppInfoType = AppInfoType::Unknown;
        AppMode mAppMode = AppMode::Unknown;
        [[nodiscard]] bool isValid() const;
    };

    explicit AppUpdateInfoJob(QObject *parent = nullptr);
    ~AppUpdateInfoJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QByteArray appsId() const;
    void setAppsId(const QByteArray &newAppsId);

    [[nodiscard]] AppInfoType appInfoType() const;
    void setAppInfoType(AppInfoType newAppInfoType);

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] AppMode appMode() const;
    void setAppMode(AppMode newAppMode);

    [[nodiscard]] QString appVersion() const;
    void setAppVersion(const QString &newAppVersion);

    [[nodiscard]] AppUpdateInfo appUpdateInfo() const;
    void setAppUpdateInfo(const AppUpdateInfo &newAppUpdateInfo);

Q_SIGNALS:
    void appUpdateInfoDone(const QJsonObject &obj);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    [[nodiscard]] LIBROCKETCHATRESTAPI_QT_NO_EXPORT QString generateUrlExtension() const;
    AppUpdateInfo mAppUpdateInfo;
    QByteArray mAppsId;
    QString mAppVersion;
    AppInfoType mAppInfoType = AppInfoType::Unknown;
    AppMode mAppMode = AppMode::Unknown;
};
}
