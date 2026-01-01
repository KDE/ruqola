/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationmanagerutils.h"
#include "utils.h"

#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;

QJsonArray AuthenticationManagerUtils::loginResume(const QString &token)
{
    QJsonArray array;
    QJsonObject resumeObject;
    resumeObject["resume"_L1] = token;
    array.append(resumeObject);
    return array;
}

QJsonArray AuthenticationManagerUtils::loginLdap(const QString &user, const QString &password)
{
    QJsonArray array;
    QJsonObject ldapObject;
    ldapObject["ldap"_L1] = true;
    ldapObject["username"_L1] = user;

    // Ldap seems to new supported hashPassword... To bad.
    ldapObject["ldapPass"_L1] = password;
    ldapObject["ldapOptions"_L1] = QJsonObject();
    array.append(ldapObject);
    return array;
}

QJsonArray AuthenticationManagerUtils::loginOAuth(const QString &credentialToken, const QString &credentialSecret)
{
    QJsonArray array;
    QJsonObject oauthObject;

    QJsonObject oauthCredentialObject;
    oauthCredentialObject["credentialToken"_L1] = credentialToken;
    oauthCredentialObject["credentialSecret"_L1] = credentialSecret;
    oauthObject["oauth"_L1] = oauthCredentialObject;
    array.append(oauthObject);
    return array;
}

QJsonObject AuthenticationManagerUtils::hashPassword(const QString &password)
{
    QJsonObject passwordObject;
    const QByteArray sha256pw = Utils::convertSha256Password(password);
    passwordObject["algorithm"_L1] = u"sha-256"_s;
    passwordObject["digest"_L1] = QLatin1StringView(sha256pw);
    return passwordObject;
}

QJsonArray AuthenticationManagerUtils::login(const QString &user, const QString &password)
{
    QJsonArray array;
    QJsonObject loginObject;

    QJsonObject userObject;
    if (user.contains(u'@')) {
        userObject["email"_L1] = user;
    } else {
        userObject["username"_L1] = user;
    }
    loginObject["user"_L1] = userObject;

    const QJsonObject passwordObject = hashPassword(password);
    loginObject["password"_L1] = passwordObject;

    array.append(loginObject);
    return array;
}

QJsonArray AuthenticationManagerUtils::sendOTP(const QString &otpCode, const QJsonObject &lastLoginPayload)
{
    QJsonArray array;
    QJsonObject otp;

    QJsonObject totpObject;
    totpObject["code"_L1] = otpCode;
    totpObject["login"_L1] = lastLoginPayload;
    otp["totp"_L1] = totpObject;
    array.append(otp);
    return array;
}
