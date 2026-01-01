/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminusersallmodel.h"

#include "ruqola_debug.h"
#include "utils.h"
#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;
AdminUsersAllModel::AdminUsersAllModel(QObject *parent)
    : AdminUsersBaseModel(parent)
{
}

AdminUsersAllModel::~AdminUsersAllModel() = default;

bool AdminUsersAllModel::setData(const QModelIndex &id, const QVariant &value, int role)
{
    if (!id.isValid()) {
        qCWarning(RUQOLA_LOG) << "ERROR: invalid index";
        return false;
    }
    switch (role) {
    case AdminUsersAllRoles::ActiveUser: {
        const int idx = id.row();
        User &user = mUsers[idx];
        user.setActive(value.toBool());
        const QModelIndex newIndex = index(idx, AdminUsersAllModel::ActiveUserDisplay);
        Q_EMIT dataChanged(newIndex, newIndex);
        return true;
    }
    case AdminUsersAllRoles::Name:
    case AdminUsersAllRoles::UserName:
    case AdminUsersAllRoles::Email:
    case AdminUsersAllRoles::Roles:
    case AdminUsersAllRoles::Status:
    case AdminUsersAllRoles::UserId:
    case AdminUsersAllRoles::ActiveUserDisplay:
    case AdminUsersAllRoles::Administrator:
        return false;
    }
    return false;
}

QString AdminUsersAllModel::createRegistrationStatus(const User &user) const
{
    const bool lastLoginIsValid = user.lastLogin().isValid();
    const QStringList usersExcludedFromPending = {"bot"_L1, "app"_L1};
    if (!lastLoginIsValid && !usersExcludedFromPending.contains(user.type())) {
        return i18n("Pending");
    } else if (user.active() && lastLoginIsValid) {
        return i18n("Active");
    } else if (!user.active() && lastLoginIsValid) {
        return i18n("Deactivated");
    }
    return {};
}

QVariant AdminUsersAllModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminUsersAllRoles>(section)) {
        case AdminUsersAllModel::Name:
            return i18n("Name");
        case AdminUsersAllModel::UserName:
            return i18n("UserName");
        case AdminUsersAllModel::Email:
            return i18n("Email");
        case AdminUsersAllModel::Roles:
            return i18n("Role");
        case AdminUsersAllModel::ActiveUserDisplay:
            return i18n("Disabled");
        case AdminUsersAllModel::Status:
            return i18n("Status");
        case AdminUsersAllModel::RegistrationStatus:
            return i18n("Registration status");
        case AdminUsersAllModel::UserId:
        case AdminUsersAllModel::ActiveUser:
        case AdminUsersAllRoles::Administrator:
            return {};
        }
    }
    return {};
}

int AdminUsersAllModel::columnCount([[maybe_unused]] const QModelIndex &parent) const
{
    constexpr int val = static_cast<int>(AdminUsersAllModel::LastColumn) + 1;
    return val;
}

QList<int> AdminUsersAllModel::hideColumns() const
{
    return {AdminUsersAllRoles::UserId, AdminUsersAllRoles::ActiveUser, AdminUsersAllRoles::Administrator};
}

QVariant AdminUsersAllModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mUsers.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const User &user = mUsers.at(index.row());
    const int col = index.column();
    switch (static_cast<AdminUsersAllRoles>(col)) {
    case AdminUsersAllRoles::Name: {
        const QString name = user.name();
        return name.isEmpty() ? user.userName() : name;
    }
    case AdminUsersAllRoles::UserName:
        return user.userName();
    case AdminUsersAllRoles::Email:
        return user.userEmailsInfo().email;
    case AdminUsersAllRoles::Roles:
        return user.i18nRoles().join(u',');
    case AdminUsersAllRoles::Status:
        return Utils::displaytextFromPresenceStatus(user.status());
    case AdminUsersAllRoles::ActiveUserDisplay:
        return user.active() ? i18n("Active") : i18n("Disabled");
    case AdminUsersAllRoles::ActiveUser:
        return user.active();
    case AdminUsersAllRoles::UserId:
        return user.userId();
    case AdminUsersAllRoles::Administrator:
        return user.roles().contains(u"admin"_s);
    case AdminUsersAllRoles::RegistrationStatus:
        return createRegistrationStatus(user);
    }
    return {};
}

#include "moc_adminusersallmodel.cpp"
