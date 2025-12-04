/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roleinfo.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(RoleInfo, Ruqola_RoleInfo)

#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
//{"_id":"admin","description":"Admin","mandatory2fa":false,"name":"admin","protected":true,"scope":"Users"},
RoleInfo::RoleInfo() = default;

RoleInfo::~RoleInfo() = default;

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
    mScope = obj["scope"_L1].toString();
    mIdentifier = obj["_id"_L1].toString();
    mName = obj["name"_L1].toString();
    mDescription = obj["description"_L1].toString();
    mRoleProtected = obj["protected"_L1].toBool();
    mMandatory2fa = obj["mandatory2fa"_L1].toBool();
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
    d.space() << "Identifier:" << t.identifier();
    d.space() << "Scope:" << t.scope();
    d.space() << "name:" << t.name();
    d.space() << "description:" << t.description();
    d.space() << "protected:" << t.roleProtected();
    d.space() << "mandatory2fa:" << t.mandatory2fa();
    return d;
}
