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
    Q_ENUMS(Method)

    explicit AuthenticationManagerBase(QObject *parent = nullptr);
    ~AuthenticationManagerBase() override;

    [[nodiscard]] bool isLoggedIn() const;
    [[nodiscard]] bool isLoggedOut() const;

    [[nodiscard]] QString userId() const;
    [[nodiscard]] QString authToken() const;
    [[nodiscard]] qint64 tokenExpires() const;

    void setUserId(const QString &newUserId);
    void setAuthToken(const QString &authToken);

    [[nodiscard]] AuthenticationManager::LoginStatus loginStatus() const;
    void setLoginStatus(AuthenticationManager::LoginStatus status);

    void loginPassword(const QString &user, const QString &password);
    void loginLDAP(const QString &user, const QString &password); // TODO: LDAP options?
    void loginOAuth(const QString &credentialToken, const QString &credentialSecret);
    void login();
    void sendOTP(const QString &otp);
    void logout();
    void logoutAndCleanup();
    void loginImpl(const QJsonArray &params);

    [[nodiscard]] QString convertMethodEnumToString(AuthenticationManagerBase::Method m);
    void processMethodResponseImpl(const QJsonObject &response, AuthenticationManagerBase::Method method);

    virtual void callLoginImpl(const QJsonArray &params, Method method) = 0;

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
