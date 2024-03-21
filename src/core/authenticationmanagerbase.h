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
