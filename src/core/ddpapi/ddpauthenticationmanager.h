/*

 * SPDX-FileCopyrightText: 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

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
    // state == LoginOngoing for all the time since the login request until a response
    //   comes back, then it may result in
    //   - LoggedIn
    //   - LoginOtpRequired
    //   - LoginFailedInvalidUserOrPassword
    //   - GenericError
    // state == LoginOtpAuthOngoing since when the otp code is sent to the server until
    //  a response comes back, then it may become:
    //   - LoggedIn
    //   - LoginFailedInvalidOtp
    //   - GenericError
    // state == LogoutOngoing since the logout request is sent until a response is received,
    //   next states could be
    //   - LoggedOut
    //   - GenericError
    // state == LogoutCleanUpOngoing since the clean up request is sent until a response is
    //   received, resulting in one of these states:
    //   - LoggedOutAndCleanedUp
    //   - GenericError
    // GenericError is used when the class doesn't know what else to do, and is irreversible
    enum LoginStatus {
        Connecting,
        LoginOngoing,
        LoggedIn,
        LoginFailedInvalidUserOrPassword,
        LoginOtpRequired,
        LoginOtpAuthOngoing,
        LoginFailedInvalidOtp,
        LogoutOngoing,
        LoggedOut,
        LogoutCleanUpOngoing,
        LoggedOutAndCleanedUp,
        FailedToLoginPluginProblem,
        LoginFailedUserNotActivated,
        LoginFailedLoginBlockForIp,
        LoginFailedLoginBlockedForUser,
        LoginFailedLoginAppNotAllowedToLogin,
        GenericError,
    };
    Q_ENUM(LoginStatus)

    explicit DDPAuthenticationManager(DDPClient *ddpClient, QObject *parent = nullptr);
    ~DDPAuthenticationManager() override;

    void login();
    void login(const QString &user, const QString &password);
    void loginLDAP(const QString &user, const QString &password); // TODO: LDAP options?
    void loginOAuth(const QString &credentialToken, const QString &credentialSecret);
    void sendOTP(const QString &otp);
    void logout();

    void setAuthToken(const QString &authToken);

    [[nodiscard]] QString userId() const;
    [[nodiscard]] QString authToken() const;
    [[nodiscard]] bool isLoggedIn() const;
    [[nodiscard]] bool isLoggedOut() const;
    [[nodiscard]] LoginStatus loginStatus() const;
    void setLoginStatus(LoginStatus newStatus);

    [[nodiscard]] qint64 tokenExpires() const;

Q_SIGNALS:
    void loginStatusChanged();

private:
    QString mUserId;
    QString mAuthToken;
    qint64 mTokenExpires;
    LoginStatus mLoginStatus = LoggedOut;
    // Used when sending OTP
    QJsonObject mLastLoginPayload;

    void processMethodResponseImpl(int operationId, const QJsonObject &response) override;

    // Authentication doesn't involve any subscriptions
    void processSubscriptionResultImpl(int subscriptionId, const QJsonObject &result) override
    {
        Q_UNUSED(subscriptionId)
        Q_UNUSED(result)
    }

    void loginImpl(const QJsonArray &params);
    void clientConnectedChangedSlot();
    [[nodiscard]] bool checkGenericError() const;
};
