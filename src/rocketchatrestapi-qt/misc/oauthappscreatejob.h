/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

#include <QMap>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT OauthAppsCreateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct OauthAppsCreateInfo {
        QString name;
        QString redirectUri;
        bool active = false;
        [[nodiscard]] bool isValid() const;
    };

    explicit OauthAppsCreateJob(QObject *parent = nullptr);
    ~OauthAppsCreateJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] OauthAppsCreateInfo oauthAppsCreateInfo() const;
    void setOauthAppsCreateInfo(const OauthAppsCreateInfo &newOauthAppsCreateInfo);

Q_SIGNALS:
    void oauthAppsCreateDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(OauthAppsCreateJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    OauthAppsCreateInfo mOauthAppsCreateInfo;
};
}
