/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT GoogleAuthJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GoogleAuthJob(QObject *parent = nullptr);
    ~GoogleAuthJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString accessToken() const;
    void setAccessToken(const QString &accessToken);

    [[nodiscard]] int expireTokenInSeconds() const;
    void setExpireTokenInSeconds(int expireTokenInSeconds);

    [[nodiscard]] QString idToken() const;
    void setIdToken(const QString &idToken);

Q_SIGNALS:
    void googleauthDone(const QString &authToken, const QString &userId);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mAccessToken;
    QString mIdToken;
    int mExpireTokenInSeconds = -1;
    // TODO identity ???? it's optional
};
}
