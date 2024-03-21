/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationmanager/authenticationmanagerbase.h"
#include "authenticationmanager/authenticationmanagerutils.h"
#include "ruqola_authentication_debug.h"
#include "utils.h"
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

QString AuthenticationManagerBase::convertMethodEnumToString(AuthenticationManagerBase::Method m)
{
    switch (m) {
    case AuthenticationManagerBase::Method::Login:
        return AuthenticationManagerBase::METHOD_LOGIN;
    case AuthenticationManagerBase::Method::SendOtp:
        return AuthenticationManagerBase::METHOD_SEND_OTP;
    case AuthenticationManagerBase::Method::Logout:
        return AuthenticationManagerBase::METHOD_LOGOUT;
    case AuthenticationManagerBase::Method::LogoutCleanUp:
        return AuthenticationManagerBase::METHOD_LOGOUT_CLEAN_UP;
    }
    return {};
}

void AuthenticationManagerBase::sendOTP(const QString &otpCode)
{
    if (checkGenericError()) {
        return;
    }

    if (mLoginStatus == AuthenticationManager::LoginStatus::LoginOtpAuthOngoing) {
        qCWarning(RUQOLA_AUTHENTICATION_LOG) << Q_FUNC_INFO << "Another OTP authentication is going on.";
        return;
    }

    //    if ((mLoginStatus != LoginStatus::LoginOtpRequired) && (mLoginStatus != LoginStatus::LoginFailedInvalidOtp)) {
    //        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "Trying to send OTP but none was requested by the server.";
    //        return;
    //    }
    callLoginImpl(AuthenticationManagerUtils::sendOTP(otpCode, mLastLoginPayload), Method::SendOtp);
    setLoginStatus(AuthenticationManager::LoginStatus::LoginOtpAuthOngoing);
}

void AuthenticationManagerBase::logout()
{
    if (checkGenericError()) {
        return;
    }

    if (mLoginStatus == AuthenticationManager::LoginStatus::LogoutOngoing) {
        qCWarning(RUQOLA_AUTHENTICATION_LOG) << Q_FUNC_INFO << "Another logout operation is ongoing.";
        return;
    }

    if (isLoggedOut()) {
        qCWarning(RUQOLA_AUTHENTICATION_LOG) << Q_FUNC_INFO << "User is already logged out.";
        return;
    }

    const QString params = sl("[]");

    callLoginImpl(Utils::strToJsonArray(params), Method::Logout);
    setLoginStatus(AuthenticationManager::LoginStatus::LogoutOngoing);
}

void AuthenticationManagerBase::logoutAndCleanup()
{
    if (checkGenericError()) {
        return;
    }

    if (mLoginStatus == AuthenticationManager::LoginStatus::LogoutOngoing) {
        qCWarning(RUQOLA_AUTHENTICATION_LOG) << Q_FUNC_INFO << "Another logout operation is ongoing.";
        return;
    }

    if (isLoggedOut()) {
        qCWarning(RUQOLA_AUTHENTICATION_LOG) << Q_FUNC_INFO << "User is already logged out.";
        return;
    }

    // TODO fix parameters! In RC client we use "user"
    const QString params = sl("[]");

    callLoginImpl(Utils::strToJsonArray(params), Method::LogoutCleanUp);
    setLoginStatus(AuthenticationManager::LoginStatus::LogoutOngoing);
}

void AuthenticationManagerBase::loginImpl(const QJsonArray &params)
{
    if (checkGenericError()) {
        return;
    }

    if (mLoginStatus == AuthenticationManager::LoginOngoing) {
        qCWarning(RUQOLA_AUTHENTICATION_LOG) << "A login operation is already ongoing, dropping request.";
        return;
    }

    if (mLoginStatus == AuthenticationManager::LoggedIn) {
        qCWarning(RUQOLA_AUTHENTICATION_LOG) << "User is already logged in on this server, ignoring.";
        return;
    }

    // TODO: sanity checks on params

    mLastLoginPayload = params[0].toObject();
    callLoginImpl(params, Method::Login);
    setLoginStatus(AuthenticationManager::LoginStatus::LoginOngoing);
}

void AuthenticationManagerBase::processMethodResponseImpl(const QJsonObject &response, AuthenticationManagerBase::Method method)
{
    switch (method) {
    case Method::Login: // intentional fall-through
    case Method::SendOtp:
        if (response.contains(sl("result"))) {
            const QJsonObject result = response[sl("result")].toObject();
            mAuthToken = result[sl("token")].toString();
            mUserId = result[sl("id")].toString();
            mTokenExpires = result[sl("tokenExpires")].toObject()[sl("$date")].toDouble();
            setLoginStatus(AuthenticationManager::LoggedIn);
        }

        if (response.contains(sl("error"))) {
            const QJsonValue errorCode = response[sl("error")].toObject()[sl("error")];
            qCWarning(RUQOLA_AUTHENTICATION_LOG) << "Login Error: " << response;
            // TODO: to be more user friendly, there would need to be more context
            // in case of a 403 error, as it may be received in different cases:
            //   - When logging in with user and password -> invalid username or password
            //   - When resuming an older login with an invalid / expired auth token -> invalid or expired token
            //   - When logging in with an invalid / expired OAuth token (e.g. google, facebook, etc.) -> invalid or expired token
            if (errorCode.isDouble() && errorCode.toInt() == 403) {
                qCWarning(RUQOLA_AUTHENTICATION_LOG) << "Invalid username or password.";
                setLoginStatus(AuthenticationManager::LoginFailedInvalidUserOrPassword);
            } else if (errorCode.isString() && errorCode.toString() == sl("totp-required")) {
                qCWarning(RUQOLA_AUTHENTICATION_LOG) << "Two factor authentication is enabled on the server."
                                                     << "A one-time password is required to complete the login procedure.";
                setLoginStatus(AuthenticationManager::LoginOtpRequired);
            } else if (errorCode.isString() && errorCode.toString() == sl("totp-invalid")) {
                qCWarning(RUQOLA_AUTHENTICATION_LOG) << "Invalid OTP code.";
                setLoginStatus(AuthenticationManager::LoginFailedInvalidOtp);
            } else if (errorCode.isString() && errorCode.toString() == sl("error-user-is-not-activated")) {
                qCWarning(RUQOLA_AUTHENTICATION_LOG) << "User is not activated.";
                setLoginStatus(AuthenticationManager::LoginFailedUserNotActivated);
            } else if (errorCode.isString() && errorCode.toString() == sl("error-login-blocked-for-ip")) {
                qCWarning(RUQOLA_AUTHENTICATION_LOG) << "Login has been temporarily blocked For IP.";
                setLoginStatus(AuthenticationManager::LoginFailedLoginBlockForIp);
            } else if (errorCode.isString() && errorCode.toString() == sl("error-login-blocked-for-user")) {
                qCWarning(RUQOLA_AUTHENTICATION_LOG) << "Login has been temporarily blocked For User.";
                setLoginStatus(AuthenticationManager::LoginFailedLoginBlockedForUser);
            } else if (errorCode.isString() && errorCode.toString() == sl("error-app-user-is-not-allowed-to-login")) {
                qCWarning(RUQOLA_AUTHENTICATION_LOG) << "App user is not allowed to login.";
                setLoginStatus(AuthenticationManager::LoginFailedLoginAppNotAllowedToLogin);
            } else {
                qCWarning(RUQOLA_AUTHENTICATION_LOG) << "Generic error during login. Couldn't process" << response;
                setLoginStatus(AuthenticationManager::GenericError);
            }
            return;
        }

        break;

    case Method::Logout:
        // Don't really expect any errors here, except maybe when logging out without
        // being logged in. That is being taken care of in DDPAuthenticationManager::logout.
        // Printing any error message that may come up just in case, and preventing any other
        // operations by switching to GenericError state.
        if (response.contains(sl("error"))) {
            qCWarning(RUQOLA_AUTHENTICATION_LOG) << "Error while logging out. Server response:" << response;
            setLoginStatus(AuthenticationManager::GenericError);
            return;
        }

        setLoginStatus(AuthenticationManager::LoggedOut);
        break;

    case Method::LogoutCleanUp:
        // Maybe the clean up request payload is corrupted
        if (response.contains(sl("error"))) {
            const QJsonValue errorCode = response[sl("error")].toObject()[sl("error")];
            qCWarning(RUQOLA_AUTHENTICATION_LOG) << "Couldn't clean up on logout. Server response:" << response << " error code " << errorCode;
            // If we get here we're likely getting something wrong from the UI.
            // Need to prevent any further operation from now on.
            setLoginStatus(AuthenticationManager::GenericError);
            return;
        }

        setLoginStatus(AuthenticationManager::LoggedOutAndCleanedUp);
        break;
    }
    qDebug() << " result " << response;
}

#undef sl
