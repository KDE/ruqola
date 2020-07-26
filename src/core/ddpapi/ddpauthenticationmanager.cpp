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

#include "ddpapi/ddpauthenticationmanager.h"

#include "ddpapi/ddpclient.h"

#include "ruqola_ddpapi_debug.h"
#include "ruqola_ddpapi_command_debug.h"

#include "utils.h"

#include <QByteArray>
#include <QJsonArray>
#include <QJsonObject>

#define sl(x) QStringLiteral(x)

QString DDPAuthenticationManager::METHOD_LOGIN = sl("login");
QString DDPAuthenticationManager::METHOD_SEND_OTP = sl("login");
QString DDPAuthenticationManager::METHOD_LOGOUT = sl("logout");
QString DDPAuthenticationManager::METHOD_LOGOUT_CLEAN_UP = sl("logoutCleanUp");

DDPAuthenticationManager::DDPAuthenticationManager(DDPClient *ddpClient, QObject *parent)
    : DDPManager(ddpClient, parent)
{
    connect(ddpClient, &DDPClient::connectedChanged,
            this, &DDPAuthenticationManager::clientConnectedChangedSlot);
    connect(ddpClient, &DDPClient::connecting,
            this, [this](){ setLoginStatus(LoginStatus::Connecting); });
}

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

    const QString params = sl(R"(
[
    {
        "resume": "%1"
    }
])").arg(mAuthToken);

    loginImpl(Utils::strToJsonArray(params));
}

void DDPAuthenticationManager::login(const QString &user, const QString &password)
{
    // TODO: need to support login with email too ("email": "address" instead of "username": "user")
    const QByteArray sha256pw = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    const QString params = sl(
        R"(
                              [
                              {
                              "user": {
                              "username": "%1"
                              },
                              "password": {
                              "digest": "%2",
                              "algorithm": "sha-256"
                              }
                              }
                              ])").arg(user, QString::fromLatin1(sha256pw.toHex()));
    loginImpl(Utils::strToJsonArray(params));
}

void DDPAuthenticationManager::loginLDAP(const QString &user, const QString &password)
{
    const QString params = sl(R"(
[
    {
        "ldap": true,
        "username": "%1",
        "ldapPass": "%2",
        "ldapOptions": {}
    }
])").arg(user, password);

    loginImpl(Utils::strToJsonArray(params));
}

void DDPAuthenticationManager::loginOAuth(const QString &credentialToken, const QString &credentialSecret)
{
    const QString params = sl(R"(
[
    {
        "oauth": {
            "credentialToken": "%1",
            "credentialSecret": "%2"
        }
    }
])").arg(credentialToken, credentialSecret);

    loginImpl(Utils::strToJsonArray(params));
}

void DDPAuthenticationManager::loginImpl(const QJsonArray &params)
{
    if (checkGenericError()) {
        return;
    }

    if (mLoginStatus == LoginOngoing) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "A login operation is already ongoing, dropping request.";
        return;
    }

    if (mLoginStatus == LoggedIn) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "User is already logged in on this server, ignoring.";
        return;
    }

    // TODO: sanity checks on params

    mLastLoginPayload = params[0].toObject();
    ddpClient()->invokeMethodAndRegister(
        METHOD_LOGIN,
        params,
        this,
        static_cast<int>(Method::Login));
    setLoginStatus(LoginStatus::LoginOngoing);
}

void DDPAuthenticationManager::sendOTP(const QString &otpCode)
{
    if (checkGenericError()) {
        return;
    }

    if (mLoginStatus == LoginStatus::LoginOtpAuthOngoing) {
        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO
                                     << "Another OTP authentication is going on.";
        return;
    }

    if ((mLoginStatus != LoginStatus::LoginOtpRequired)
        && (mLoginStatus != LoginStatus::LoginFailedInvalidOtp)) {
        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO
                                     << "Trying to send OTP but none was requested by the server.";
        return;
    }

    const QString params = sl(R"(
[
    {
        "totp": {
            "login": %1,
            "code": "%2"
        }
    }
])").arg(QLatin1String(QJsonDocument(mLastLoginPayload).toJson().constData()), otpCode);

    ddpClient()->invokeMethodAndRegister(
        METHOD_SEND_OTP,
        Utils::strToJsonArray(params),
        this,
        static_cast<int>(Method::SendOtp));
    setLoginStatus(LoginStatus::LoginOtpAuthOngoing);
}

void DDPAuthenticationManager::logout()
{
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

    ddpClient()->invokeMethodAndRegister(
        METHOD_LOGOUT,
        Utils::strToJsonArray(params),
        this,
        static_cast<int>(Method::SendOtp));
    setLoginStatus(LoginStatus::LogoutOngoing);
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
            setLoginStatus(LoggedIn);
        }

        if (response.contains(sl("error"))) {
            const QJsonValue errorCode = response[sl("error")].toObject()
                                         [sl("error")];

            // TODO: to be more user friendly, there would need to be more context
            // in case of a 403 error, as it may be received in different cases:
            //   - When logging in with user and password -> invalid username or password
            //   - When resuming an older login with an invalid / expired auth token -> invalid or expired token
            //   - When logging in with an invalid / expired OAuth token (e.g. google, facebook, etc.) -> invalid or expired token
            if (errorCode.isDouble() && errorCode.toInt() == 403) {
                qCWarning(RUQOLA_DDPAPI_LOG) << "Invalid username or password.";
                setLoginStatus(LoginFailedInvalidUserOrPassword);
            } else if (errorCode.isString() && errorCode.toString() == sl("totp-required")) {
                qCWarning(RUQOLA_DDPAPI_LOG) << "Two factor authentication is enabled on the server."
                                             << "A one-time password is required to complete the login procedure.";
                setLoginStatus(LoginOtpRequired);
            } else if (errorCode.isString() && errorCode.toString() == sl("totp-invalid")) {
                qCWarning(RUQOLA_DDPAPI_LOG) << "Invalid OTP code.";
                setLoginStatus(LoginFailedInvalidOtp);
            } else {
                qCWarning(RUQOLA_DDPAPI_LOG) << "Generic error during login. Couldn't process" << response;
                setLoginStatus(GenericError);
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
            qCWarning(RUQOLA_DDPAPI_LOG) << "Error while logging out. Server response:"
                                         << response;
            setLoginStatus(GenericError);
            return;
        }

        setLoginStatus(LoggedOut);
        break;

    case Method::LogoutCleanUp:
        // Maybe the clean up request payload is corrupted
        if (response.contains(sl("error"))) {
            const QJsonValue errorCode = response[sl("error")].toObject()
                                         [sl("error")];
            qCWarning(RUQOLA_DDPAPI_LOG) << "Couldn't clean up on logout. Server response:"
                                         << response;
            // If we get here we're likely getting something wrong from the UI.
            // Need to prevent any further operation from now on.
            setLoginStatus(GenericError);
            return;
        }

        setLoginStatus(LoggedOutAndCleanedUp);
        break;
    }
}

DDPAuthenticationManager::LoginStatus DDPAuthenticationManager::loginStatus() const
{
    return mLoginStatus;
}

bool DDPAuthenticationManager::isLoggedIn() const
{
    return mLoginStatus == DDPAuthenticationManager::LoggedIn;
}

bool DDPAuthenticationManager::isLoggedOut() const
{
    return mLoginStatus == DDPAuthenticationManager::LoggedOut
           || mLoginStatus == DDPAuthenticationManager::LogoutCleanUpOngoing
           || mLoginStatus == DDPAuthenticationManager::LoggedOutAndCleanedUp;
}

void DDPAuthenticationManager::setLoginStatus(DDPAuthenticationManager::LoginStatus status)
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
    if (mLoginStatus == DDPAuthenticationManager::FailedToLoginPluginProblem) {
        return;
    }
    if (checkGenericError()) {
        return;
    }

    // Just connected -> not logged in yet -> state = LoggedOut
    // Just disconnected -> whatever state we're in, need to change to LoggedOut
    setLoginStatus(LoginStatus::LoggedOut);
}

bool DDPAuthenticationManager::checkGenericError() const
{
    if (mLoginStatus == LoginStatus::GenericError) {
        qCWarning(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO
                                     << "The authentication manager is in an irreversible error state and can't perform any operation.";
    }

    return mLoginStatus == LoginStatus::GenericError;
}

#undef sl
