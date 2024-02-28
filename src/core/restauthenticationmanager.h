/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "authenticationmanager.h"
#include "libruqolacore_export.h"
#include <QJsonObject>
#include <QObject>
namespace RocketChatRestApi
{
class Connection;
}
class LIBRUQOLACORE_EXPORT RESTAuthenticationManager : public QObject
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
    explicit RESTAuthenticationManager(RocketChatRestApi::Connection *restApiConnection, QObject *parent = nullptr);
    ~RESTAuthenticationManager() override;

    void login();
    void login(const QString &user, const QString &password);
    void loginLDAP(const QString &user, const QString &password); // TODO: LDAP options?
    void loginOAuth(const QString &credentialToken, const QString &credentialSecret);
    void sendOTP(const QString &otp);
    void logout();

    void setAuthToken(const QString &authToken);

    void setLoginStatus(AuthenticationManager::LoginStatus status);

    [[nodiscard]] bool isLoggedIn() const;
    [[nodiscard]] bool isLoggedOut() const;

    [[nodiscard]] QString userId() const;
    [[nodiscard]] QString authToken() const;
    [[nodiscard]] qint64 tokenExpires() const;

Q_SIGNALS:
    void loginStatusChanged();

private:
    LIBRUQOLACORE_NO_EXPORT void loginImpl(const QJsonArray &params);
    LIBRUQOLACORE_NO_EXPORT void processMethodResponseImpl(const QJsonObject &replyObject, Method method);
    LIBRUQOLACORE_NO_EXPORT QJsonObject generateJsonMethod(const QString &method, const QJsonDocument &params, quint64 id);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT bool checkGenericError() const;
    QString mAuthToken;
    QString mUserId;
    qint64 mTokenExpires;
    RocketChatRestApi::Connection *const mRestApiConnection;
    AuthenticationManager::LoginStatus mLoginStatus = AuthenticationManager::LoggedOut;
    // Used when sending OTP
    QJsonObject mLastLoginPayload;
    int mIndex = 5000;
};
