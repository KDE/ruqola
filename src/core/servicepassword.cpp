/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "servicepassword.h"
#include <QDebug>

ServicePassword::ServicePassword()
{
}

ServicePassword::~ServicePassword()
{
}

bool ServicePassword::operator ==(const ServicePassword &other) const
{
    return mEmail2faEnabled == other.email2faEnabled()
           && mPassword == other.password()
           && mTotp == other.totp();
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

QDebug operator <<(QDebug d, const ServicePassword &t)
{
    d << "mEmail2faEnabled " << t.email2faEnabled();
    d << "mPassword " << t.password();
    d << "mTotp " << t.totp();
    return d;
}
