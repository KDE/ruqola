/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#include "authenticationinfo.h"

AuthenticationInfo::AuthenticationInfo()
{
}

AuthenticationInfo::~AuthenticationInfo()
{
}

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

QDebug operator <<(QDebug d, const AuthenticationInfo &t)
{
    d << "Iconname: " << t.iconName();
    d << "Name: " << t.name();
    d << "Type: " << t.oauthType();
    return d;
}
