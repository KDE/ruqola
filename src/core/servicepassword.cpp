/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servicepassword.h"

ServicePassword::ServicePassword() = default;

ServicePassword::~ServicePassword() = default;

bool ServicePassword::operator==(const ServicePassword &other) const
{
    return mEmail2faEnabled == other.email2faEnabled() && mPassword == other.password() && mTotp == other.totp();
}

void ServicePassword::parseService(const QJsonObject &replyObject)
{
    const QJsonObject email2faJson = replyObject.value(QStringLiteral("email2fa")).toObject();
    if (!email2faJson.isEmpty()) {
        mEmail2faEnabled = email2faJson.value(QStringLiteral("enabled")).toBool();
    }
    const QJsonObject passwordJson = replyObject.value(QStringLiteral("password")).toObject();
    if (!passwordJson.isEmpty()) {
        mPassword = passwordJson.value(QStringLiteral("bcrypt")).toString();
    }
    const QJsonObject totpJson = replyObject.value(QStringLiteral("totp")).toObject();
    if (!totpJson.isEmpty()) {
        mTotp = totpJson.value(QStringLiteral("enabled")).toBool();
    }
}

bool ServicePassword::email2faEnabled() const
{
    return mEmail2faEnabled;
}

void ServicePassword::setEmail2faEnabled(bool email2faEnabled)
{
    mEmail2faEnabled = email2faEnabled;
}

QString ServicePassword::password() const
{
    return mPassword;
}

void ServicePassword::setPassword(const QString &password)
{
    mPassword = password;
}

bool ServicePassword::totp() const
{
    return mTotp;
}

void ServicePassword::setTotp(bool totp)
{
    mTotp = totp;
}

QDebug operator<<(QDebug d, const ServicePassword &t)
{
    d << "mEmail2faEnabled " << t.email2faEnabled();
    d << "mPassword " << t.password();
    d << "mTotp " << t.totp();
    return d;
}
