/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
#include <QJsonArray>
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
        ActionButton,
        Icon,
    };

    struct AppUpdateInfo {
        QJsonArray mAppsSettings;
        QByteArray mAppsId;
        QString mAppVersion;
        QString mStatus;
        AppInfoType mAppInfoType = AppInfoType::Unknown;
        AppMode mAppMode = AppMode::Unknown;
        [[nodiscard]] bool isValid() const;
    };

    explicit AppUpdateInfoJob(QObject *parent = nullptr);
    ~AppUpdateInfoJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] AppUpdateInfo appUpdateInfo() const;
    void setAppUpdateInfo(const AppUpdateInfo &newAppUpdateInfo);

Q_SIGNALS:
    void appUpdateInfoDone(const QJsonObject &obj);
    void fetchActionButtonsDone(const QJsonArray &obj);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onDeleteRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    [[nodiscard]] LIBROCKETCHATRESTAPI_QT_NO_EXPORT QString generateUrlExtension() const;
    AppUpdateInfo mAppUpdateInfo;
};
}
