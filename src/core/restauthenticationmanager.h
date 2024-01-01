/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QObject>
namespace RocketChatRestApi
{
class Connection;
}
class LIBRUQOLACORE_EXPORT RESTAuthenticationManager : public QObject
{
    Q_OBJECT
public:
    explicit RESTAuthenticationManager(RocketChatRestApi::Connection *restApiConnection, QObject *parent = nullptr);
    ~RESTAuthenticationManager() override;

    void login();
    void login(const QString &user, const QString &password);
    void loginLDAP(const QString &user, const QString &password); // TODO: LDAP options?
    void loginOAuth(const QString &credentialToken, const QString &credentialSecret);
    void sendOTP(const QString &otp);
    void logout();

    void setAuthToken(const QString &authToken);

private:
    QString mAuthToken;
    RocketChatRestApi::Connection *const mRestApiConnection;
};
