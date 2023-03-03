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
    resumeObject[QStringLiteral("resume")] = token;
    array.append(resumeObject);
    return array;
}

QJsonArray DDPAuthenticationManagerUtils::loginLdap(const QString &user, const QString &password)
{
    QJsonArray array;
    QJsonObject ldapObject;
    ldapObject[QStringLiteral("ldap")] = true;
    ldapObject[QStringLiteral("username")] = user;

    // Ldap seems to new supported hashPassword... To bad.
    ldapObject[QStringLiteral("ldapPass")] = password;
    ldapObject[QStringLiteral("ldapOptions")] = QJsonObject();
    array.append(ldapObject);
    return array;
}

QJsonArray DDPAuthenticationManagerUtils::loginOAuth(const QString &credentialToken, const QString &credentialSecret)
{
    QJsonArray array;
    QJsonObject oauthObject;

    QJsonObject oauthCredentialObject;
    oauthCredentialObject[QStringLiteral("credentialToken")] = credentialToken;
    oauthCredentialObject[QStringLiteral("credentialSecret")] = credentialSecret;
    oauthObject[QStringLiteral("oauth")] = oauthCredentialObject;
    array.append(oauthObject);
    return array;
}

QJsonObject DDPAuthenticationManagerUtils::hashPassword(const QString &password)
{
    QJsonObject passwordObject;
    const QByteArray sha256pw = Utils::convertSha256Password(password);
    passwordObject[QStringLiteral("algorithm")] = QStringLiteral("sha-256");
    passwordObject[QStringLiteral("digest")] = QString::fromLatin1(sha256pw);
    return passwordObject;
}

QJsonArray DDPAuthenticationManagerUtils::login(const QString &user, const QString &password)
{
    QJsonArray array;
    QJsonObject loginObject;

    QJsonObject userObject;
    if (user.contains(QLatin1Char('@'))) {
        userObject[QStringLiteral("email")] = user;
    } else {
        userObject[QStringLiteral("username")] = user;
    }
    loginObject[QStringLiteral("user")] = userObject;

    const QJsonObject passwordObject = hashPassword(password);
    loginObject[QStringLiteral("password")] = passwordObject;

    array.append(loginObject);
    return array;
}

QJsonArray DDPAuthenticationManagerUtils::sendOTP(const QString &otpCode, const QJsonObject &lastLoginPayload)
{
    QJsonArray array;
    QJsonObject otp;

    QJsonObject totpObject;
    totpObject[QStringLiteral("code")] = otpCode;
    totpObject[QStringLiteral("login")] = lastLoginPayload;
    otp[QStringLiteral("totp")] = totpObject;
    array.append(otp);
    return array;
}
