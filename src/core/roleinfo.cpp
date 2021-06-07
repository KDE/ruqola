/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "roleinfo.h"

RoleInfo::RoleInfo()
{
    //{"_id":"admin","description":"Admin","mandatory2fa":false,"name":"admin","protected":true,"scope":"Users"},
}

const QString &RoleInfo::identifier() const
{
    return mIdentifier;
}

void RoleInfo::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

const QString &RoleInfo::scope() const
{
    return mScope;
}

void RoleInfo::setScope(const QString &newScope)
{
    mScope = newScope;
}

void RoleInfo::parseRoleInfo(const QJsonObject &obj)
{
    mScope = obj[QLatin1String("scope")].toString();
    mIdentifier = obj[QLatin1String("_id")].toString();
    mName = obj[QLatin1String("name")].toString();
}

const QString &RoleInfo::name() const
{
    return mName;
}

void RoleInfo::setName(const QString &newName)
{
    mName = newName;
}

// TODO translate name.

QDebug operator<<(QDebug d, const RoleInfo &t)
{
    // TODO add more
    d << "Identifier: " << t.identifier();
    d << "Scope: " << t.scope();
    d << "name: " << t.name();
    return d;
}
