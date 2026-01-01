/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT AppInfoJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    enum class AppInfoType {
        Unknown,
        Versions,
        Logs,
        Apis,
        ScreenShots,
        Settings,
        Languages,
    };

    explicit AppInfoJob(QObject *parent = nullptr);
    ~AppInfoJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QByteArray appsId() const;
    void setAppsId(const QByteArray &newAppsId);

    [[nodiscard]] AppInfoType appInfoType() const;
    void setAppInfoType(AppInfoType newAppInfoType);

    [[nodiscard]] bool canStart() const override;

Q_SIGNALS:
    void appInfoDone(const QJsonObject &obj);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    [[nodiscard]] LIBROCKETCHATRESTAPI_QT_NO_EXPORT QString generateUrlExtension() const;
    QByteArray mAppsId;
    AppInfoType mAppInfoType = AppInfoType::Unknown;
};
}
