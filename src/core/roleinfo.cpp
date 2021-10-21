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

bool RoleInfo::operator==(const RoleInfo &other) const
{
    return mScope == other.scope() && mIdentifier == other.identifier() && mName == other.name() && mDescription == other.description() && mRoleProtected
        && other.roleProtected() && mMandatory2fa == other.mandatory2fa();
}

void RoleInfo::parseRoleInfo(const QJsonObject &obj)
{
    // TODO updateAt!
    mScope = obj[QLatin1String("scope")].toString();
    mIdentifier = obj[QLatin1String("_id")].toString();
    mName = obj[QLatin1String("name")].toString();
    mDescription = obj[QLatin1String("description")].toString();
    mRoleProtected = obj[QLatin1String("protected")].toBool();
    mMandatory2fa = obj[QLatin1String("mandatory2fa")].toBool();
}

const QString &RoleInfo::name() const
{
    return mName;
}

void RoleInfo::setName(const QString &newName)
{
    mName = newName;
}

const QString &RoleInfo::description() const
{
    return mDescription;
}

void RoleInfo::setDescription(const QString &newDescription)
{
    mDescription = newDescription;
}

bool RoleInfo::roleProtected() const
{
    return mRoleProtected;
}

void RoleInfo::setRoleProtected(bool newRoleProtected)
{
    mRoleProtected = newRoleProtected;
}

bool RoleInfo::mandatory2fa() const
{
    return mMandatory2fa;
}

void RoleInfo::setMandatory2fa(bool newMandatory2fa)
{
    mMandatory2fa = newMandatory2fa;
}

// TODO translate name.

QDebug operator<<(QDebug d, const RoleInfo &t)
{
    // TODO add more
    d << "Identifier: " << t.identifier();
    d << "Scope: " << t.scope();
    d << "name: " << t.name();
    d << "description: " << t.description();
    d << "protected: " << t.roleProtected();
    d << "mandatory2fa: " << t.mandatory2fa();
    return d;
}
