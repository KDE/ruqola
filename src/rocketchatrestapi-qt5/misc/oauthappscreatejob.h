/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QMap>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT OauthAppsCreateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct OauthAppsCreateInfo {
        QString name;
        QString redirectUri;
        bool active = false;
        Q_REQUIRED_RESULT bool isValid() const;
    };

    explicit OauthAppsCreateJob(QObject *parent = nullptr);
    ~OauthAppsCreateJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT OauthAppsCreateInfo oauthAppsCreateInfo() const;
    void setOauthAppsCreateInfo(const OauthAppsCreateInfo &newOauthAppsCreateInfo);

Q_SIGNALS:
    void oauthAppsCreateDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(OauthAppsCreateJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    OauthAppsCreateInfo mOauthAppsCreateInfo;
};
}
