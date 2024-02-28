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
    // TODO info.messageObj;
    // Use generateJsonMethod
    job->setMethodCallJobInfo(std::move(info));
    mRestApiConnection->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::MethodCallJob::methodCallDone, this, &RESTAuthenticationManager::processMethodResponseImpl);

    if (!job->start()) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "Impossible to start MethodCallJob::login job";
    }
}

void RESTAuthenticationManager::processMethodResponseImpl(const QJsonObject &replyObject)
{
    qDebug() << " result " << replyObject;
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
#if 0
    if (checkGenericError()) {
        return;
    }

    if (mLoginStatus == LoginStatus::LoginOtpAuthOngoing) {
        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "Another OTP authentication is going on.";
        return;
    }

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
#if 0
    if (checkGenericError()) {
        return;
    }

    if (mLoginStatus == LoginStatus::LogoutOngoing) {
        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "Another logout operation is ongoing.";
        return;
    }

    if (isLoggedOut()) {
        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "User is already logged out.";
        return;
    }

    const QString params = sl("[]");

    ddpClient()->invokeMethodAndRegister(METHOD_LOGOUT, Utils::strToJsonArray(params), this, static_cast<int>(Method::SendOtp));
    setLoginStatus(LoginStatus::LogoutOngoing);
#endif
    // mRestApiConnection->logout();
}
#undef sl

#include "moc_restauthenticationmanager.cpp"
