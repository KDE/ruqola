/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roles.h"
#include "ruqola_debug.h"

#include <QJsonArray>
using namespace Qt::Literals::StringLiterals;
Roles::Roles() = default;

QList<Role> Roles::roles() const
{
    return mRoles;
}

bool Roles::isEmpty() const
{
    return mRoles.isEmpty();
}

void Roles::setRoles(const QList<Role> &roles)
{
    mRoles = roles;
}

void Roles::updateRoles(const QJsonObject &obj)
{
    const QString type = obj["type"_L1].toString();
    const QString id = obj["_id"_L1].toString();
    const QByteArray userId = obj["u"_L1].toObject().value("_id"_L1).toString().toLatin1();
    bool foundUser = false;
    // qDebug() << " type " << type << " id " << id << " userId" << userId;
    if (type == "added"_L1) {
        for (int i = 0, total = mRoles.count(); i < total; ++i) {
            if (mRoles.at(i).userId() == userId) {
                Role &r = mRoles[i];
                r.updateRole(id, true);
                foundUser = true;
                break;
            }
        }
        if (!foundUser) {
            Role r;
            r.setUserId(userId);
            r.updateRole(id, true);
            mRoles.append(std::move(r));
        }
    } else if (type == "removed"_L1) {
        for (int i = 0, total = mRoles.count(); i < total; ++i) {
            if (mRoles.at(i).userId() == userId) {
                Role r = mRoles.takeAt(i);
                r.updateRole(id, false);
                if (r.hasARole()) {
                    mRoles.append(std::move(r));
                }
                foundUser = true;
                break;
            }
        }
        if (!foundUser) {
            qCWarning(RUQOLA_LOG) << "Problem you want to remove role for an not existing role! it seems to be a bug ";
        }
    } else {
        qCWarning(RUQOLA_LOG) << "Unknown change role type " << type;
    }
}

int Roles::count() const
{
    return mRoles.count();
}

Role Roles::at(int index) const
{
    if (index < 0 || index >= mRoles.count()) {
        qCWarning(RUQOLA_LOG) << "Invalid index " << index;
        return {};
    }
    return mRoles.at(index);
}

void Roles::parseRole(const QJsonObject &obj)
{
    mRoles.clear();

    const QJsonArray roleArray = obj["roles"_L1].toArray();
    const auto roleArrayCount = roleArray.count();
    mRoles.reserve(roleArrayCount);
    for (auto i = 0; i < roleArrayCount; ++i) {
        Role r;
        r.parseRole(roleArray.at(i).toObject());
        if (r.isValid()) {
            mRoles.append(std::move(r));
        } else {
            qCWarning(RUQOLA_LOG) << "Invalid role" << roleArray.at(i).toObject();
        }
    }
}

Role Roles::findRoleByUserId(const QByteArray &userId) const
{
    for (const Role &r : std::as_const(mRoles)) {
        if (r.userId() == userId) {
            return r;
        }
    }
    return {};
}

QDebug operator<<(QDebug d, const Roles &t)
{
    d.space() << "count" << t.count();
    for (int i = 0; i < t.count(); i++) {
        d.space() << t.at(i) << "\n";
    }
    return d;
}
