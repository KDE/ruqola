/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationmanagerbase.h"
#include "ddpapi/authenticationmanagerutils.h"
#include "ruqola_authentication_debug.h"
#define sl(x) QStringLiteral(x)

QString AuthenticationManagerBase::METHOD_LOGIN = sl("login");
QString AuthenticationManagerBase::METHOD_SEND_OTP = sl("login");
QString AuthenticationManagerBase::METHOD_LOGOUT = sl("logout");
QString AuthenticationManagerBase::METHOD_LOGOUT_CLEAN_UP = sl("logoutCleanUp");

AuthenticationManagerBase::AuthenticationManagerBase(QObject *parent)
    : QObject{parent}
{
}

AuthenticationManagerBase::~AuthenticationManagerBase() = default;

bool AuthenticationManagerBase::isLoggedIn() const
{
    return mLoginStatus == AuthenticationManager::LoggedIn;
}

bool AuthenticationManagerBase::isLoggedOut() const
{
    return mLoginStatus == AuthenticationManager::LoggedOut || mLoginStatus == AuthenticationManager::LogoutCleanUpOngoing
        || mLoginStatus == AuthenticationManager::LoggedOutAndCleanedUp;
}

void AuthenticationManagerBase::setLoginStatus(AuthenticationManager::LoginStatus status)
{
    if (mLoginStatus != status) {
        mLoginStatus = status;
        Q_EMIT loginStatusChanged();
    }
}

void AuthenticationManagerBase::setAuthToken(const QString &authToken)
{
    mAuthToken = authToken;
}

AuthenticationManager::LoginStatus AuthenticationManagerBase::loginStatus() const
{
    return mLoginStatus;
}

QString AuthenticationManagerBase::userId() const
{
    return mUserId;
}

QString AuthenticationManagerBase::authToken() const
{
    return mAuthToken;
}

qint64 AuthenticationManagerBase::tokenExpires() const
{
    return mTokenExpires;
}

bool AuthenticationManagerBase::checkGenericError() const
{
    if (mLoginStatus == AuthenticationManager::LoginStatus::GenericError) {
        qCWarning(RUQOLA_AUTHENTICATION_LOG) << Q_FUNC_INFO << "The authentication manager is in an irreversible error state and can't perform any operation.";
    }

    return mLoginStatus == AuthenticationManager::LoginStatus::GenericError;
}

void AuthenticationManagerBase::loginPassword(const QString &user, const QString &password)
{
    loginImpl(AuthenticationManagerUtils::login(user, password));
}

void AuthenticationManagerBase::loginLDAP(const QString &user, const QString &password)
{
    loginImpl(AuthenticationManagerUtils::loginLdap(user, password));
}

void AuthenticationManagerBase::loginOAuth(const QString &credentialToken, const QString &credentialSecret)
{
    loginImpl(AuthenticationManagerUtils::loginOAuth(credentialToken, credentialSecret));
}

void AuthenticationManagerBase::login()
{
    if (mAuthToken.isNull()) {
        qCWarning(RUQOLA_AUTHENTICATION_LOG) << "No auth token available, can't login.";
        return;
    }

    loginImpl(AuthenticationManagerUtils::loginResume(mAuthToken));
}

#undef sl
