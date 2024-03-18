/*

 * SPDX-FileCopyrightText: 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "ddpapi/ddpauthenticationmanager.h"

#include "ddpapi/authenticationmanagerutils.h"
#include "ddpapi/ddpclient.h"

#include "ruqola_ddpapi_debug.h"

#include "utils.h"

#include <QJsonArray>

#define sl(x) QStringLiteral(x)

QString DDPAuthenticationManager::METHOD_LOGIN = sl("login");
QString DDPAuthenticationManager::METHOD_SEND_OTP = sl("login");
QString DDPAuthenticationManager::METHOD_LOGOUT = sl("logout");
QString DDPAuthenticationManager::METHOD_LOGOUT_CLEAN_UP = sl("logoutCleanUp");

DDPAuthenticationManager::DDPAuthenticationManager(DDPClient *ddpClient, QObject *parent)
    : DDPManager(ddpClient, parent)
{
    connect(ddpClient, &DDPClient::connectedChanged, this, &DDPAuthenticationManager::clientConnectedChangedSlot);
    connect(ddpClient, &DDPClient::connecting, this, [this]() {
        setLoginStatus(AuthenticationManager::LoginStatus::Connecting);
    });
}

DDPAuthenticationManager::~DDPAuthenticationManager() = default;

void DDPAuthenticationManager::setAuthToken(const QString &authToken)
{
    mAuthToken = authToken;
}

void DDPAuthenticationManager::login()
{
    if (mAuthToken.isNull()) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "No auth token available, can't login.";
        return;
    }

    loginImpl(AuthenticationManagerUtils::loginResume(mAuthToken));
}

void DDPAuthenticationManager::login(const QString &user, const QString &password)
{
    loginImpl(AuthenticationManagerUtils::login(user, password));
}

void DDPAuthenticationManager::loginLDAP(const QString &user, const QString &password)
{
    loginImpl(AuthenticationManagerUtils::loginLdap(user, password));
}

void DDPAuthenticationManager::loginOAuth(const QString &credentialToken, const QString &credentialSecret)
{
    loginImpl(AuthenticationManagerUtils::loginOAuth(credentialToken, credentialSecret));
}

void DDPAuthenticationManager::loginImpl(const QJsonArray &params)
{
    if (checkGenericError()) {
        return;
    }

    if (mLoginStatus == AuthenticationManager::LoginOngoing) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "A login operation is already ongoing, dropping request.";
        return;
    }

    if (mLoginStatus == AuthenticationManager::LoggedIn) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "User is already logged in on this server, ignoring.";
        return;
    }

    // TODO: sanity checks on params

    mLastLoginPayload = params[0].toObject();
    ddpClient()->invokeMethodAndRegister(METHOD_LOGIN, params, this, static_cast<int>(Method::Login));
    setLoginStatus(AuthenticationManager::LoginStatus::LoginOngoing);
}

void DDPAuthenticationManager::sendOTP(const QString &otpCode)
{
    if (checkGenericError()) {
        return;
    }

    if (mLoginStatus == AuthenticationManager::LoginStatus::LoginOtpAuthOngoing) {
        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "Another OTP authentication is going on.";
        return;
    }

    //    if ((mLoginStatus != LoginStatus::LoginOtpRequired) && (mLoginStatus != LoginStatus::LoginFailedInvalidOtp)) {
    //        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "Trying to send OTP but none was requested by the server.";
    //        return;
    //    }
    ddpClient()->invokeMethodAndRegister(METHOD_SEND_OTP,
                                         AuthenticationManagerUtils::sendOTP(otpCode, mLastLoginPayload),
                                         this,
                                         static_cast<int>(Method::SendOtp));
    setLoginStatus(AuthenticationManager::LoginStatus::LoginOtpAuthOngoing);
}

void DDPAuthenticationManager::logout()
{
    if (checkGenericError()) {
        return;
    }

    if (mLoginStatus == AuthenticationManager::LoginStatus::LogoutOngoing) {
        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "Another logout operation is ongoing.";
        return;
    }

    if (isLoggedOut()) {
        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "User is already logged out.";
        return;
    }

    const QString params = sl("[]");

    ddpClient()->invokeMethodAndRegister(METHOD_LOGOUT, Utils::strToJsonArray(params), this, static_cast<int>(Method::Logout));
    setLoginStatus(AuthenticationManager::LoginStatus::LogoutOngoing);
}

void DDPAuthenticationManager::logoutAndCleanup()
{
    if (checkGenericError()) {
        return;
    }

    if (mLoginStatus == AuthenticationManager::LoginStatus::LogoutOngoing) {
        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "Another logout operation is ongoing.";
        return;
    }

    if (isLoggedOut()) {
        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "User is already logged out.";
        return;
    }

    // TODO fix parameters! In RC client we use "user"
    const QString params = sl("[]");

    ddpClient()->invokeMethodAndRegister(METHOD_LOGOUT_CLEAN_UP, Utils::strToJsonArray(params), this, static_cast<int>(Method::LogoutCleanUp));
    setLoginStatus(AuthenticationManager::LoginStatus::LogoutOngoing);
}

QString DDPAuthenticationManager::userId() const
{
    return mUserId;
}

QString DDPAuthenticationManager::authToken() const
{
    return mAuthToken;
}

void DDPAuthenticationManager::processMethodResponseImpl(int operationId, const QJsonObject &response)
{
    switch (static_cast<Method>(operationId)) {
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
            qCWarning(RUQOLA_DDPAPI_LOG) << "Login Error: " << response;
            // TODO: to be more user friendly, there would need to be more context
            // in case of a 403 error, as it may be received in different cases:
            //   - When logging in with user and password -> invalid username or password
            //   - When resuming an older login with an invalid / expired auth token -> invalid or expired token
            //   - When logging in with an invalid / expired OAuth token (e.g. google, facebook, etc.) -> invalid or expired token
            if (errorCode.isDouble() && errorCode.toInt() == 403) {
                qCWarning(RUQOLA_DDPAPI_LOG) << "Invalid username or password.";
                setLoginStatus(AuthenticationManager::LoginFailedInvalidUserOrPassword);
            } else if (errorCode.isString() && errorCode.toString() == sl("totp-required")) {
                qCWarning(RUQOLA_DDPAPI_LOG) << "Two factor authentication is enabled on the server."
                                             << "A one-time password is required to complete the login procedure.";
                setLoginStatus(AuthenticationManager::LoginOtpRequired);
            } else if (errorCode.isString() && errorCode.toString() == sl("totp-invalid")) {
                qCWarning(RUQOLA_DDPAPI_LOG) << "Invalid OTP code.";
                setLoginStatus(AuthenticationManager::LoginFailedInvalidOtp);
            } else if (errorCode.isString() && errorCode.toString() == sl("error-user-is-not-activated")) {
                qCWarning(RUQOLA_DDPAPI_LOG) << "User is not activated.";
                setLoginStatus(AuthenticationManager::LoginFailedUserNotActivated);
            } else if (errorCode.isString() && errorCode.toString() == sl("error-login-blocked-for-ip")) {
                qCWarning(RUQOLA_DDPAPI_LOG) << "Login has been temporarily blocked For IP.";
                setLoginStatus(AuthenticationManager::LoginFailedLoginBlockForIp);
            } else if (errorCode.isString() && errorCode.toString() == sl("error-login-blocked-for-user")) {
                qCWarning(RUQOLA_DDPAPI_LOG) << "Login has been temporarily blocked For User.";
                setLoginStatus(AuthenticationManager::LoginFailedLoginBlockedForUser);
            } else if (errorCode.isString() && errorCode.toString() == sl("error-app-user-is-not-allowed-to-login")) {
                qCWarning(RUQOLA_DDPAPI_LOG) << "App user is not allowed to login.";
                setLoginStatus(AuthenticationManager::LoginFailedLoginAppNotAllowedToLogin);
            } else {
                qCWarning(RUQOLA_DDPAPI_LOG) << "Generic error during login. Couldn't process" << response;
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
            qCWarning(RUQOLA_DDPAPI_LOG) << "Error while logging out. Server response:" << response;
            setLoginStatus(AuthenticationManager::GenericError);
            return;
        }

        setLoginStatus(AuthenticationManager::LoggedOut);
        break;

    case Method::LogoutCleanUp:
        // Maybe the clean up request payload is corrupted
        if (response.contains(sl("error"))) {
            const QJsonValue errorCode = response[sl("error")].toObject()[sl("error")];
            qCWarning(RUQOLA_DDPAPI_LOG) << "Couldn't clean up on logout. Server response:" << response << " error code " << errorCode;
            // If we get here we're likely getting something wrong from the UI.
            // Need to prevent any further operation from now on.
            setLoginStatus(AuthenticationManager::GenericError);
            return;
        }

        setLoginStatus(AuthenticationManager::LoggedOutAndCleanedUp);
        break;
    }
}

AuthenticationManager::LoginStatus DDPAuthenticationManager::loginStatus() const
{
    return mLoginStatus;
}

bool DDPAuthenticationManager::isLoggedIn() const
{
    return mLoginStatus == AuthenticationManager::LoggedIn;
}

bool DDPAuthenticationManager::isLoggedOut() const
{
    return mLoginStatus == AuthenticationManager::LoggedOut || mLoginStatus == AuthenticationManager::LogoutCleanUpOngoing
        || mLoginStatus == AuthenticationManager::LoggedOutAndCleanedUp;
}

void DDPAuthenticationManager::setLoginStatus(AuthenticationManager::LoginStatus status)
{
    if (mLoginStatus != status) {
        mLoginStatus = status;
        Q_EMIT loginStatusChanged();
    }
}

qint64 DDPAuthenticationManager::tokenExpires() const
{
    return mTokenExpires;
}

void DDPAuthenticationManager::clientConnectedChangedSlot()
{
    if (mLoginStatus == AuthenticationManager::FailedToLoginPluginProblem) {
        return;
    }
    if (checkGenericError()) {
        return;
    }
    // Just connected -> not logged in yet -> state = LoggedOut
    // Just disconnected -> whatever state we're in, need to change to LoggedOut
    setLoginStatus(AuthenticationManager::LoginStatus::LoggedOut);
}

bool DDPAuthenticationManager::checkGenericError() const
{
    if (mLoginStatus == AuthenticationManager::LoginStatus::GenericError) {
        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "The authentication manager is in an irreversible error state and can't perform any operation.";
    }

    return mLoginStatus == AuthenticationManager::LoginStatus::GenericError;
}

#undef sl

#include "moc_ddpauthenticationmanager.cpp"
