/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roleinfo.h"
#include <QJsonObject>

//{"_id":"admin","description":"Admin","mandatory2fa":false,"name":"admin","protected":true,"scope":"Users"},
RoleInfo::RoleInfo() = default;

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
