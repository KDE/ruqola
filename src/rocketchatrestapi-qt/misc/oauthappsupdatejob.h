/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT OauthAppsUpdateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct OauthAppsUpdateInfo {
        QString name;
        QString redirectUri;
        QString appId;
        bool active = false;
        [[nodiscard]] bool isValid() const;
    };

    explicit OauthAppsUpdateJob(QObject *parent = nullptr);
    ~OauthAppsUpdateJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] OauthAppsUpdateInfo oauthAppsUpdateInfo() const;
    void setOauthAppsUpdateInfo(const OauthAppsUpdateInfo &newOauthAppsUpdateInfo);

Q_SIGNALS:
    void oauthAppsUpdateDone(const QJsonObject &replyObject);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    OauthAppsUpdateInfo mOauthAppsUpdateInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::OauthAppsUpdateJob::OauthAppsUpdateInfo, Q_RELOCATABLE_TYPE);
