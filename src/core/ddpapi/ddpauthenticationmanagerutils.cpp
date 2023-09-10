/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ddpauthenticationmanagerutils.h"
#include "utils.h"

#include <QJsonObject>

QJsonArray DDPAuthenticationManagerUtils::loginResume(const QString &token)
{
    QJsonArray array;
    QJsonObject resumeObject;
    resumeObject[QLatin1String("resume")] = token;
    array.append(resumeObject);
    return array;
}

QJsonArray DDPAuthenticationManagerUtils::loginLdap(const QString &user, const QString &password)
{
    QJsonArray array;
    QJsonObject ldapObject;
    ldapObject[QLatin1String("ldap")] = true;
    ldapObject[QLatin1String("username")] = user;

    // Ldap seems to new supported hashPassword... To bad.
    ldapObject[QLatin1String("ldapPass")] = password;
    ldapObject[QLatin1String("ldapOptions")] = QJsonObject();
    array.append(ldapObject);
    return array;
}

QJsonArray DDPAuthenticationManagerUtils::loginOAuth(const QString &credentialToken, const QString &credentialSecret)
{
    QJsonArray array;
    QJsonObject oauthObject;

    QJsonObject oauthCredentialObject;
    oauthCredentialObject[QLatin1String("credentialToken")] = credentialToken;
    oauthCredentialObject[QLatin1String("credentialSecret")] = credentialSecret;
    oauthObject[QLatin1String("oauth")] = oauthCredentialObject;
    array.append(oauthObject);
    return array;
}

QJsonObject DDPAuthenticationManagerUtils::hashPassword(const QString &password)
{
    QJsonObject passwordObject;
    const QByteArray sha256pw = Utils::convertSha256Password(password);
    passwordObject[QLatin1String("algorithm")] = QStringLiteral("sha-256");
    passwordObject[QLatin1String("digest")] = QString::fromLatin1(sha256pw);
    return passwordObject;
}

QJsonArray DDPAuthenticationManagerUtils::login(const QString &user, const QString &password)
{
    QJsonArray array;
    QJsonObject loginObject;

    QJsonObject userObject;
    if (user.contains(QLatin1Char('@'))) {
        userObject[QLatin1String("email")] = user;
    } else {
        userObject[QLatin1String("username")] = user;
    }
    loginObject[QLatin1String("user")] = userObject;

    const QJsonObject passwordObject = hashPassword(password);
    loginObject[QLatin1String("password")] = passwordObject;

    array.append(loginObject);
    return array;
}

QJsonArray DDPAuthenticationManagerUtils::sendOTP(const QString &otpCode, const QJsonObject &lastLoginPayload)
{
    QJsonArray array;
    QJsonObject otp;

    QJsonObject totpObject;
    totpObject[QLatin1String("code")] = otpCode;
    totpObject[QLatin1String("login")] = lastLoginPayload;
    otp[QLatin1String("totp")] = totpObject;
    array.append(otp);
    return array;
}
