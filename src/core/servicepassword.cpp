/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servicepassword.h"

#include <QJsonObject>

QT_IMPL_METATYPE_EXTERN_TAGGED(ServicePassword, Ruqola_ServicePassword)

using namespace Qt::Literals::StringLiterals;
ServicePassword::ServicePassword() = default;

ServicePassword::~ServicePassword() = default;

bool ServicePassword::operator==(const ServicePassword &other) const
{
    return mEmail2faEnabled == other.email2faEnabled() && mPassword == other.password() && mTotp == other.totp();
}

void ServicePassword::parseService(const QJsonObject &replyObject)
{
    const QJsonObject email2faJson = replyObject.value("email2fa"_L1).toObject();
    if (!email2faJson.isEmpty()) {
        mEmail2faEnabled = email2faJson.value("enabled"_L1).toBool();
    }
    const QJsonObject passwordJson = replyObject.value("password"_L1).toObject();
    if (!passwordJson.isEmpty()) {
        mPassword = passwordJson.value("bcrypt"_L1).toString();
    }
    const QJsonObject totpJson = replyObject.value("totp"_L1).toObject();
    if (!totpJson.isEmpty()) {
        mTotp = totpJson.value("enabled"_L1).toBool();
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
    d.space() << "mEmail2faEnabled" << t.email2faEnabled();
    d.space() << "mPassword" << t.password();
    d.space() << "mTotp" << t.totp();
    return d;
}
