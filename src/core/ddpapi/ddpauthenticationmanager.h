/*

 * Copyright 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
    // state == LogoutOngoing since the logout requet is sent until a response is received,
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
    // Not implemented yet
    void logoutCleanup();

    void setAuthToken(const QString &authToken);

    Q_REQUIRED_RESULT QString userId() const;
    Q_REQUIRED_RESULT QString authToken() const;
    Q_REQUIRED_RESULT bool isLoggedIn() const;
    Q_REQUIRED_RESULT bool isLoggedOut() const;
    Q_REQUIRED_RESULT LoginStatus loginStatus() const;
    void setLoginStatus(LoginStatus newStatus);

    Q_REQUIRED_RESULT qint64 tokenExpires() const;

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
    Q_REQUIRED_RESULT bool checkGenericError() const;
};
