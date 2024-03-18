/*

 * SPDX-FileCopyrightText: 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "authenticationmanager.h"
#include "ddpapi/ddpmanager.h"
#include "libruqolacore_export.h"

#include <QJsonObject>

class LIBRUQOLACORE_EXPORT DDPAuthenticationManager : public DDPManager
{
    Q_OBJECT

    enum class Method {
        Login,
        SendOtp,
        Logout,
        LogoutCleanUp,
    };

    static QString METHOD_LOGIN;
    static QString METHOD_SEND_OTP;
    static QString METHOD_LOGOUT;
    static QString METHOD_LOGOUT_CLEAN_UP;

public:
    explicit DDPAuthenticationManager(DDPClient *ddpClient, QObject *parent = nullptr);
    ~DDPAuthenticationManager() override;

    void login();
    void login(const QString &user, const QString &password);
    void loginLDAP(const QString &user, const QString &password); // TODO: LDAP options?
    void loginOAuth(const QString &credentialToken, const QString &credentialSecret);
    void sendOTP(const QString &otp);
    void logout();
    void logoutAndCleanup();

    void setAuthToken(const QString &authToken);

    [[nodiscard]] QString userId() const;
    [[nodiscard]] QString authToken() const;
    [[nodiscard]] bool isLoggedIn() const;
    [[nodiscard]] bool isLoggedOut() const;
    [[nodiscard]] AuthenticationManager::LoginStatus loginStatus() const;
    void setLoginStatus(AuthenticationManager::LoginStatus newStatus);

    [[nodiscard]] qint64 tokenExpires() const;

Q_SIGNALS:
    void loginStatusChanged();

private:
    QString mUserId;
    QString mAuthToken;
    qint64 mTokenExpires;
    AuthenticationManager::LoginStatus mLoginStatus = AuthenticationManager::LoggedOut;
    // Used when sending OTP
    QJsonObject mLastLoginPayload;

    LIBRUQOLACORE_NO_EXPORT void processMethodResponseImpl(int operationId, const QJsonObject &response) override;

    // Authentication doesn't involve any subscriptions
    LIBRUQOLACORE_NO_EXPORT void processSubscriptionResultImpl(int subscriptionId, const QJsonObject &result) override
    {
        Q_UNUSED(subscriptionId)
        Q_UNUSED(result)
    }

    LIBRUQOLACORE_NO_EXPORT void loginImpl(const QJsonArray &params);
    LIBRUQOLACORE_NO_EXPORT void clientConnectedChangedSlot();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT bool checkGenericError() const;
};
