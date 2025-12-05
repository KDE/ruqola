/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolesmanager.h"

#include "ruqola_debug.h"
#include <QJsonArray>

#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
RolesManager::RolesManager(QObject *parent)
    : QObject{parent}
{
}

RolesManager::~RolesManager() = default;

void RolesManager::parseRoles(const QJsonObject &obj)
{
    const QJsonArray array = obj["roles"_L1].toArray();

    mRoleInfo.reserve(array.count());
    for (const QJsonValue &current : array) {
        const QJsonObject roleObject = current.toObject();
        RoleInfo info;
        info.parseRoleInfo(roleObject);
        mRoleInfo.append(std::move(info));
    }
}

void RolesManager::updateRoles(const QJsonArray &contents)
{
    bool wasChanged = false;
    for (const QJsonValue &current : contents) {
        const QJsonObject roleObject = current.toObject();
        const QString type = roleObject.value("type"_L1).toString();
        const QString identifier = roleObject.value("_id"_L1).toString();
        if (type == "removed"_L1) {
            for (int i = 0; i < mRoleInfo.count(); ++i) {
                if (mRoleInfo.at(i).identifier() == identifier) {
                    mRoleInfo.removeAt(i);
                    wasChanged = true;
                    break;
                }
            }
        } else if (type == "changed"_L1) {
            bool found = false;
            RoleInfo info;
            info.parseRoleInfo(roleObject);
            for (int i = 0, total = mRoleInfo.count(); i < total; ++i) {
                if (mRoleInfo.at(i).identifier() == identifier) {
                    mRoleInfo.removeAt(i);
                    mRoleInfo.append(std::move(info));
                    found = true;
                    wasChanged = true;
                    break;
                }
            }
            if (!found) { // Insert it.
                mRoleInfo.append(std::move(info));
                wasChanged = true;
            }
        } else {
            qCWarning(RUQOLA_LOG) << " No defined type" << type;
        }
    }
    if (wasChanged) {
        Q_EMIT rolesChanged();
    }
    // QJsonObject({"args":[{"_id":"vFXCWG9trXLti6xQm","name":"vFXCWG9trXLti6xQm","type":"removed"}],"eventName":"roles"})
    // QJsonObject({"args":[{"_id":"hiafuM2enNapgD2mg","_updatedAt":{"$date":1634588706596},"description":"","mandatory2fa":false,"name":"test4","protected":false,"scope":"Users","type":"changed"}],"eventName":"roles"})
}

const QList<RoleInfo> &RolesManager::roleInfo() const
{
    return mRoleInfo;
}

#include "moc_rolesmanager.cpp"
