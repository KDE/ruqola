/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

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

AuthenticationManager::OauthType AuthenticationInfo::oauthType() const
{
    return mOauthType;
}

void AuthenticationInfo::setOauthType(AuthenticationManager::OauthType oauthType)
{
    mOauthType = oauthType;
}

bool AuthenticationInfo::isValid() const
{
    return (mOauthType != AuthenticationManager::Unknown) && !mName.isEmpty();
}

QDebug operator<<(QDebug d, const AuthenticationInfo &t)
{
    d << "Iconname: " << t.iconName();
    d << "Name: " << t.name();
    d << "Type: " << t.oauthType();
    return d;
}

#include "moc_authenticationinfo.cpp"
