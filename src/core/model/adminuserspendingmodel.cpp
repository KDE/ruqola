/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminuserspendingmodel.h"

#include "ruqola_debug.h"
#include "utils.h"
#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;
AdminUsersPendingModel::AdminUsersPendingModel(QObject *parent)
    : AdminUsersBaseModel(parent)
{
}

AdminUsersPendingModel::~AdminUsersPendingModel() = default;

bool AdminUsersPendingModel::setData(const QModelIndex &id, const QVariant &value, int role)
{
    if (!id.isValid()) {
        qCWarning(RUQOLA_LOG) << "ERROR: invalid index";
        return false;
    }
    switch (role) {
    case AdminUsersPendingRoles::ActiveUser: {
        const int idx = id.row();
        User &user = mUsers[idx];
        user.setActive(value.toBool());
        const QModelIndex newIndex = index(idx, AdminUsersPendingModel::ActiveUserDisplay);
        Q_EMIT dataChanged(newIndex, newIndex);
        return true;
    }
    case AdminUsersPendingRoles::Name:
    case AdminUsersPendingRoles::UserName:
    case AdminUsersPendingRoles::Email:
    case AdminUsersPendingRoles::Roles:
    case AdminUsersPendingRoles::Status:
    case AdminUsersPendingRoles::UserId:
    case AdminUsersPendingRoles::ActiveUserDisplay:
    case AdminUsersPendingRoles::Administrator:
    case AdminUsersPendingRoles::PendingActionButton:
    case AdminUsersPendingModel::PendingActionButtonText:
        return false;
    }
    return false;
}

QString AdminUsersPendingModel::createPendingAction(const User &user) const
{
    return user.active() ? i18n("User first log in") : i18n("Activation");
}

QString AdminUsersPendingModel::createPendingButtonText(const User &user) const
{
    return user.active() ? i18n("Resend Welcome Email") : i18n("Activate");
}

QVariant AdminUsersPendingModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminUsersPendingRoles>(section)) {
        case AdminUsersPendingModel::Name:
            return i18n("Name");
        case AdminUsersPendingModel::UserName:
            return i18n("UserName");
        case AdminUsersPendingModel::Email:
            return i18n("Email");
        case AdminUsersPendingModel::Roles:
            return i18n("Role");
        case AdminUsersPendingModel::ActiveUserDisplay:
            return i18n("Disabled");
        case AdminUsersPendingModel::Status:
            return i18n("Status");
        case AdminUsersPendingModel::PendingActionInfo:
            return i18n("Pending action");
        case AdminUsersPendingModel::UserId:
        case AdminUsersPendingModel::ActiveUser:
        case AdminUsersPendingRoles::Administrator:
        case AdminUsersPendingRoles::PendingActionButton:
        case AdminUsersPendingModel::PendingActionButtonText:
            return {};
        }
    }
    return {};
}

int AdminUsersPendingModel::columnCount([[maybe_unused]] const QModelIndex &parent) const
{
    constexpr int val = static_cast<int>(AdminUsersPendingModel::LastColumn) + 1;
    return val;
}

QList<int> AdminUsersPendingModel::hideColumns() const
{
    return {AdminUsersPendingRoles::UserId,
            AdminUsersPendingRoles::ActiveUser,
            AdminUsersPendingRoles::Administrator,
            AdminUsersPendingRoles::Status,
            AdminUsersPendingModel::PendingActionButtonText};
}

QVariant AdminUsersPendingModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mUsers.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const User &user = mUsers.at(index.row());
    const int col = index.column();
    switch (static_cast<AdminUsersPendingRoles>(col)) {
    case AdminUsersPendingRoles::Name: {
        const QString name = user.name();
        return name.isEmpty() ? user.userName() : name;
    }
    case AdminUsersPendingRoles::UserName:
        return user.userName();
    case AdminUsersPendingRoles::Email:
        return user.userEmailsInfo().email;
    case AdminUsersPendingRoles::Roles:
        return user.i18nRoles().join(u',');
    case AdminUsersPendingRoles::Status:
        return Utils::displaytextFromPresenceStatus(user.status());
    case AdminUsersPendingRoles::ActiveUserDisplay:
        return user.active() ? i18n("Active") : i18n("Disabled");
    case AdminUsersPendingRoles::ActiveUser:
        return user.active();
    case AdminUsersPendingRoles::UserId:
        return user.userId();
    case AdminUsersPendingRoles::Administrator:
        return user.roles().contains(u"admin"_s);
    case AdminUsersPendingRoles::PendingActionInfo:
        return createPendingAction(user);
    case AdminUsersPendingRoles::PendingActionButton:
        return {};
    case AdminUsersPendingModel::PendingActionButtonText:
        return createPendingButtonText(user);
    }
    return {};
}

#include "moc_adminuserspendingmodel.cpp"
