/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permission.h"

#include "utils.h"
#include <QJsonObject>

QT_IMPL_METATYPE_EXTERN_TAGGED(Permission, Ruqola_Permission)
using namespace Qt::Literals::StringLiterals;
Permission::Permission() = default;

bool Permission::parsePermission(const QJsonObject &replyObject, const QList<RoleInfo> &roleInfo, bool restApi)
{
    // Don't store settings value.
    if (!replyObject.value("settingId"_L1).toString().isEmpty()) {
        return false;
    }
    mIdentifier = replyObject.value("_id"_L1).toString();
    if (restApi) {
        mUpdatedAt = Utils::parseIsoDate(u"_updatedAt"_s, replyObject);
    } else {
        mUpdatedAt = Utils::parseDate(u"_updatedAt"_s, replyObject);
    }
    const QJsonArray roleArray = replyObject.value("roles"_L1).toArray();
    const auto roleArrayCount{roleArray.count()};
    mRoles.reserve(roleArrayCount);
    for (int i = 0; i < roleArrayCount; ++i) {
        const QString role{roleArray.at(i).toString()};
        mRoles.append(role);
        if (roleInfo.isEmpty()) {
            mRolesStr.append(role);
        } else {
            auto index = std::find_if(roleInfo.begin(), roleInfo.end(), [role](const RoleInfo &info) {
                return (role == info.identifier());
            });
            if (index != roleInfo.end()) {
                mRolesStr.append((*index).name());
            }
        }
    }
    return true;
}

QStringList Permission::roles() const
{
    return mRoles;
}

void Permission::setRoles(const QStringList &newRoles)
{
    mRoles = newRoles;
}

qint64 Permission::updatedAt() const
{
    return mUpdatedAt;
}

void Permission::setUpdatedAt(qint64 newUpdatedAt)
{
    mUpdatedAt = newUpdatedAt;
}

bool Permission::isValid() const
{
    return mUpdatedAt != -1;
}

const QString &Permission::identifier() const
{
    return mIdentifier;
}

void Permission::setIdentifier(const QString &newIdentifier)
{
    mIdentifier = newIdentifier;
}

const QStringList &Permission::rolesStr() const
{
    return mRolesStr;
}

bool Permission::operator==(const Permission &other) const
{
    return other.roles() == roles() && other.updatedAt() == updatedAt() && other.identifier() == identifier();
}

QDebug operator<<(QDebug d, const Permission &t)
{
    d.space() << "roles:" << t.roles();
    d.space() << "rolesStr:" << t.rolesStr();
    d.space() << "mUpdatedAt:" << t.updatedAt();
    d.space() << "mIdentifier:" << t.identifier();
    return d;
}
