/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationinfo.h"

AuthenticationInfo::AuthenticationInfo() = default;

AuthenticationInfo::~AuthenticationInfo() = default;

QString AuthenticationInfo::name() const
{
    return mName;
}

void AuthenticationInfo::setName(const QString &name)
{
    mName = name;
}

QString AuthenticationInfo::iconName() const
{
    return mIconName;
}

void AuthenticationInfo::setIconName(const QString &iconName)
{
    mIconName = iconName;
}

AuthenticationManager::AuthMethodType AuthenticationInfo::oauthType() const
{
    return mOauthType;
}

void AuthenticationInfo::setOauthType(AuthenticationManager::AuthMethodType oauthType)
{
    mOauthType = oauthType;
}

bool AuthenticationInfo::isValid() const
{
    return (mOauthType != AuthenticationManager::Unknown) && !mName.isEmpty();
}

QDebug operator<<(QDebug d, const AuthenticationInfo &t)
{
    d.space() << "Iconname:" << t.iconName();
    d.space() << "Name:" << t.name();
    d.space() << "Type:" << t.oauthType();
    return d;
}

#include "moc_authenticationinfo.cpp"
