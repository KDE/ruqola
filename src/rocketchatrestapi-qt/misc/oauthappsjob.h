/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT OauthAppsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit OauthAppsJob(QObject *parent = nullptr);
    ~OauthAppsJob() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] const QString &appId() const;
    void setAppId(const QString &newAppId);

    [[nodiscard]] const QString &clientId() const;
    void setClientId(const QString &newClientId);

Q_SIGNALS:
    void oauthAppDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(OauthAppsJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mAppId;
    QString mClientId;
};
}
