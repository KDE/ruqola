/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationmanagerutils.h"
#include "utils.h"

#include <QJsonObject>
QJsonArray AuthenticationManagerUtils::loginResume(const QString &token)
{
    QJsonArray array;
    QJsonObject resumeObject;
    resumeObject[QLatin1StringView("resume")] = token;
    array.append(resumeObject);
    return array;
}

QJsonArray AuthenticationManagerUtils::loginLdap(const QString &user, const QString &password)
{
    QJsonArray array;
    QJsonObject ldapObject;
    ldapObject[QLatin1StringView("ldap")] = true;
    ldapObject[QLatin1StringView("username")] = user;

    // Ldap seems to new supported hashPassword... To bad.
    ldapObject[QLatin1StringView("ldapPass")] = password;
    ldapObject[QLatin1StringView("ldapOptions")] = QJsonObject();
    array.append(ldapObject);
    return array;
}

QJsonArray AuthenticationManagerUtils::loginOAuth(const QString &credentialToken, const QString &credentialSecret)
{
    QJsonArray array;
    QJsonObject oauthObject;

    QJsonObject oauthCredentialObject;
    oauthCredentialObject[QLatin1StringView("credentialToken")] = credentialToken;
    oauthCredentialObject[QLatin1StringView("credentialSecret")] = credentialSecret;
    oauthObject[QLatin1StringView("oauth")] = oauthCredentialObject;
    array.append(oauthObject);
    return array;
}

QJsonObject AuthenticationManagerUtils::hashPassword(const QString &password)
{
    QJsonObject passwordObject;
    const QByteArray sha256pw = Utils::convertSha256Password(password);
    passwordObject[QLatin1StringView("algorithm")] = QStringLiteral("sha-256");
    passwordObject[QLatin1StringView("digest")] = QLatin1StringView(sha256pw);
    return passwordObject;
}

QJsonArray AuthenticationManagerUtils::login(const QString &user, const QString &password)
{
    QJsonArray array;
    QJsonObject loginObject;

    QJsonObject userObject;
    if (user.contains(QLatin1Char('@'))) {
        userObject[QLatin1StringView("email")] = user;
    } else {
        userObject[QLatin1StringView("username")] = user;
    }
    loginObject[QLatin1StringView("user")] = userObject;

    const QJsonObject passwordObject = hashPassword(password);
    loginObject[QLatin1StringView("password")] = passwordObject;

    array.append(loginObject);
    return array;
}

QJsonArray AuthenticationManagerUtils::sendOTP(const QString &otpCode, const QJsonObject &lastLoginPayload)
{
    QJsonArray array;
    QJsonObject otp;

    QJsonObject totpObject;
    totpObject[QLatin1StringView("code")] = otpCode;
    totpObject[QLatin1StringView("login")] = lastLoginPayload;
    otp[QLatin1StringView("totp")] = totpObject;
    array.append(otp);
    return array;
}
