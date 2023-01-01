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
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GoogleAuthJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GoogleAuthJob(QObject *parent = nullptr);
    ~GoogleAuthJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;
    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString accessToken() const;
    void setAccessToken(const QString &accessToken);

    Q_REQUIRED_RESULT int expireTokenInSeconds() const;
    void setExpireTokenInSeconds(int expireTokenInSeconds);

    Q_REQUIRED_RESULT QString idToken() const;
    void setIdToken(const QString &idToken);

Q_SIGNALS:
    void googleauthDone(const QString &authToken, const QString &userId);

private:
    Q_DISABLE_COPY(GoogleAuthJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mAccessToken;
    QString mIdToken;
    int mExpireTokenInSeconds = -1;
    // TODO identity ???? it's optional
};
}
