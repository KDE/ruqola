/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "authenticationmanager.h"
#include "libruqolacore_export.h"
#include <QObject>

class LIBRUQOLACORE_EXPORT AuthenticationManagerBase : public QObject
{
    Q_OBJECT
public:
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

    explicit AuthenticationManagerBase(QObject *parent = nullptr);
    ~AuthenticationManagerBase() override;

    [[nodiscard]] bool isLoggedIn() const;
    [[nodiscard]] bool isLoggedOut() const;

    [[nodiscard]] QString userId() const;
    [[nodiscard]] QString authToken() const;
    [[nodiscard]] qint64 tokenExpires() const;

    void setAuthToken(const QString &authToken);

    [[nodiscard]] AuthenticationManager::LoginStatus loginStatus() const;
    void setLoginStatus(AuthenticationManager::LoginStatus status);

    void loginPassword(const QString &user, const QString &password);
    void loginLDAP(const QString &user, const QString &password); // TODO: LDAP options?
    void loginOAuth(const QString &credentialToken, const QString &credentialSecret);
    void login();

    virtual void loginImpl(const QJsonArray &params) = 0;

    virtual void sendOTP(const QString &otp) = 0;
    virtual void logout() = 0;
    virtual void logoutAndCleanup() = 0;

    [[nodiscard]] QString convertMethodEnumToString(AuthenticationManagerBase::Method m);

Q_SIGNALS:
    void loginStatusChanged();

protected:
    [[nodiscard]] bool checkGenericError() const;
    // Used when sending OTP
    QJsonObject mLastLoginPayload;

    QString mAuthToken;
    QString mUserId;
    qint64 mTokenExpires;
    AuthenticationManager::LoginStatus mLoginStatus = AuthenticationManager::LoggedOut;
};
