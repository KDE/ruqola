/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT TwitterAuthJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TwitterAuthJob(QObject *parent = nullptr);
    ~TwitterAuthJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString accessToken() const;
    void setAccessToken(const QString &accessToken);

    [[nodiscard]] QString secret() const;
    void setSecret(const QString &secret);

    [[nodiscard]] int expireTokenInSeconds() const;
    void setExpireTokenInSeconds(int expireTokenInSeconds);

    [[nodiscard]] QString appSecret() const;
    void setAppSecret(const QString &appSecret);

    [[nodiscard]] QString appId() const;
    void setAppId(const QString &appId);

Q_SIGNALS:
    void twitterDone(const QString &authToken, const QString &userId);

private:
    Q_DISABLE_COPY(TwitterAuthJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mAccessToken;
    QString mSecret;
    QString mAppSecret;
    QString mAppId;
    int mExpireTokenInSeconds = -1;
    // TODO identity ???? it's optional
};
}
