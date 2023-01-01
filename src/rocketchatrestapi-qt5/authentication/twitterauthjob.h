/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT TwitterAuthJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TwitterAuthJob(QObject *parent = nullptr);
    ~TwitterAuthJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;
    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString accessToken() const;
    void setAccessToken(const QString &accessToken);

    Q_REQUIRED_RESULT QString secret() const;
    void setSecret(const QString &secret);

    Q_REQUIRED_RESULT int expireTokenInSeconds() const;
    void setExpireTokenInSeconds(int expireTokenInSeconds);

    Q_REQUIRED_RESULT QString appSecret() const;
    void setAppSecret(const QString &appSecret);

    Q_REQUIRED_RESULT QString appId() const;
    void setAppId(const QString &appId);

Q_SIGNALS:
    void twitterDone(const QString &authToken, const QString &userId);

private:
    Q_DISABLE_COPY(TwitterAuthJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mAccessToken;
    QString mSecret;
    QString mAppSecret;
    QString mAppId;
    int mExpireTokenInSeconds = -1;
    // TODO identity ???? it's optional
};
}
