/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restauthenticationmanager.h"
#include "connection.h"
#include "ddpapi/ddpauthenticationmanagerutils.h"
#include "misc/methodcalljob.h"
#include "ruqola_ddpapi_debug.h" // TODO change it.

// We use method.callAnon here.
// We use same params as ddpclient login method.
#define sl(x) QStringLiteral(x)
QString RESTAuthenticationManager::METHOD_LOGIN = sl("login");
QString RESTAuthenticationManager::METHOD_SEND_OTP = sl("login");
QString RESTAuthenticationManager::METHOD_LOGOUT = sl("logout");
QString RESTAuthenticationManager::METHOD_LOGOUT_CLEAN_UP = sl("logoutCleanUp");

RESTAuthenticationManager::RESTAuthenticationManager(RocketChatRestApi::Connection *restApiConnection, QObject *parent)
    : QObject(parent)
    , mRestApiConnection(restApiConnection)
{
}

RESTAuthenticationManager::~RESTAuthenticationManager() = default;

void RESTAuthenticationManager::setAuthToken(const QString &authToken)
{
    mAuthToken = authToken;
}

QString RESTAuthenticationManager::userId() const
{
    return mUserId;
}

QString RESTAuthenticationManager::authToken() const
{
    return mAuthToken;
}

qint64 RESTAuthenticationManager::tokenExpires() const
{
    return mTokenExpires;
}

void RESTAuthenticationManager::login()
{
    if (mAuthToken.isNull()) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "No auth token available, can't login.";
        return;
    }

    loginImpl(DDPAuthenticationManagerUtils::loginResume(mAuthToken));
}

// TODO remove duplicate code.
QJsonObject RESTAuthenticationManager::generateJsonMethod(const QString &method, const QJsonDocument &params, quint64 id)
{
    QJsonObject json;
    json[QLatin1String("msg")] = QStringLiteral("method");
    json[QLatin1String("method")] = method;
    json[QLatin1String("id")] = QString::number(id);

    if (!params.isEmpty()) {
        if (params.isArray()) {
            json[QLatin1String("params")] = params.array();
        } else if (params.isObject()) {
            QJsonArray arr;
            arr.append(params.object());
            json[QLatin1String("params")] = arr;
        }
    }
    return json;
}

void RESTAuthenticationManager::loginImpl(const QJsonArray &params)
{
    auto job = new RocketChatRestApi::MethodCallJob(this);
    RocketChatRestApi::MethodCallJob::MethodCallJobInfo info;
    info.methodName = METHOD_LOGIN;
    info.anonymous = true;
    // TODO: verify mIndex
    info.messageObj = generateJsonMethod(info.methodName, QJsonDocument(params), mIndex++);
    job->setMethodCallJobInfo(std::move(info));
    mRestApiConnection->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::MethodCallJob::methodCallDone, this, [this](const QJsonObject &replyObject) {
        processMethodResponseImpl(replyObject, Method::Login);
    });

    if (!job->start()) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "Impossible to start MethodCallJob::login job";
    }
}

void RESTAuthenticationManager::processMethodResponseImpl(const QJsonObject &response, RESTAuthenticationManager::Method method)
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
    qDebug() << " result " << response;
}

void RESTAuthenticationManager::login(const QString &user, const QString &password)
{
    loginImpl(DDPAuthenticationManagerUtils::login(user, password));
}

void RESTAuthenticationManager::loginLDAP(const QString &user, const QString &password)
{
    loginImpl(DDPAuthenticationManagerUtils::loginLdap(user, password));
}

void RESTAuthenticationManager::loginOAuth(const QString &credentialToken, const QString &credentialSecret)
{
    loginImpl(DDPAuthenticationManagerUtils::loginOAuth(credentialToken, credentialSecret));
}

void RESTAuthenticationManager::sendOTP(const QString &otp)
{
    if (checkGenericError()) {
        return;
    }

    if (mLoginStatus == AuthenticationManager::LoginStatus::LoginOtpAuthOngoing) {
        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "Another OTP authentication is going on.";
        return;
    }

#if 0
    //    if ((mLoginStatus != LoginStatus::LoginOtpRequired) && (mLoginStatus != LoginStatus::LoginFailedInvalidOtp)) {
    //        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "Trying to send OTP but none was requested by the server.";
    //        return;
    //    }
    ddpClient()->invokeMethodAndRegister(METHOD_SEND_OTP,
                                         DDPAuthenticationManagerUtils::sendOTP(otpCode, mLastLoginPayload),
                                         this,
                                         static_cast<int>(Method::SendOtp));
    setLoginStatus(LoginStatus::LoginOtpAuthOngoing);
#endif
    Q_UNUSED(otp);
}

bool RESTAuthenticationManager::isLoggedIn() const
{
    return mLoginStatus == AuthenticationManager::LoggedIn;
}

bool RESTAuthenticationManager::isLoggedOut() const
{
    return mLoginStatus == AuthenticationManager::LoggedOut || mLoginStatus == AuthenticationManager::LogoutCleanUpOngoing
        || mLoginStatus == AuthenticationManager::LoggedOutAndCleanedUp;
}

void RESTAuthenticationManager::setLoginStatus(AuthenticationManager::LoginStatus status)
{
    if (mLoginStatus != status) {
        mLoginStatus = status;
        Q_EMIT loginStatusChanged();
    }
}

bool RESTAuthenticationManager::checkGenericError() const
{
    if (mLoginStatus == AuthenticationManager::LoginStatus::GenericError) {
        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "The authentication manager is in an irreversible error state and can't perform any operation.";
    }

    return mLoginStatus == AuthenticationManager::LoginStatus::GenericError;
}

void RESTAuthenticationManager::logout()
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
#if 0

    const QString params = sl("[]");

    ddpClient()->invokeMethodAndRegister(METHOD_LOGOUT, Utils::strToJsonArray(params), this, static_cast<int>(Method::SendOtp));
    setLoginStatus(LoginStatus::LogoutOngoing);
#endif
    // mRestApiConnection->logout();
}
#undef sl

#include "moc_restauthenticationmanager.cpp"
