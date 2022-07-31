/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminusersmodel.h"
#include "ruqola_debug.h"
#include "utils.h"
#include <KLocalizedString>

AdminUsersModel::AdminUsersModel(QObject *parent)
    : DirectoryUsersModel(parent)
{
}

AdminUsersModel::~AdminUsersModel() = default;

Users::ParseType AdminUsersModel::parseType() const
{
    return Users::ParseType::Administrator;
}

void AdminUsersModel::insertElement(const QJsonObject &obj)
{
    User user;
    const QJsonObject userObj = obj[QLatin1String("user")].toObject();
    user.parseUserRestApi(userObj, {});
    if (user.isValid()) {
        const int numberOfElement = mUsers.count();
        mUsers.appendUser(user); // TODO useful ?
        beginInsertRows(QModelIndex(), numberOfElement, mUsers.count() - 1);
        endInsertRows();
        checkFullList(); // TODO verify it
    }
}

void AdminUsersModel::removeElement(const QString &identifier)
{
    const int userCount = mUsers.count();
    for (int i = 0; i < userCount; ++i) {
        if (mUsers.at(i).userId() == identifier) {
            beginRemoveRows(QModelIndex(), i, i);
            mUsers.takeAt(i);
            mUsers.setTotal(mUsers.count()); // Update total
            endRemoveRows();
            Q_EMIT totalChanged();
            break;
        }
    }
}

void AdminUsersModel::updateElement(const QJsonObject &userObj)
{
    const int roomCount = mUsers.count();
    const QJsonObject obj = userObj[QStringLiteral("user")].toObject();
    const QString identifier{obj.value(QStringLiteral("_id")).toString()};
    for (int i = 0; i < roomCount; ++i) {
        if (mUsers.at(i).userId() == identifier) {
            beginRemoveRows(QModelIndex(), i, i);
            mUsers.takeAt(i);
            endRemoveRows();
            User newUser;
            newUser.parseUserRestApi(obj, {} /*, mRocketChatAccount->roleInfo()*/); // TODO necessary ?
            beginInsertRows(QModelIndex(), i, i);
            mUsers.insertUser(i, newUser);
            endInsertRows();
            break;
        }
    }
}

bool AdminUsersModel::setData(const QModelIndex &id, const QVariant &value, int role)
{
    if (!id.isValid()) {
        qCWarning(RUQOLA_LOG) << "ERROR: invalid index";
        return false;
    }
    const int idx = id.row();
    User &user = mUsers[idx];
    switch (role) {
    case AdminUsersRoles::ActiveUser: {
        user.setActive(value.toBool());
        const QModelIndex newIndex = index(id.row(), AdminUsersModel::ActiveUserDisplay);
        Q_EMIT dataChanged(newIndex, newIndex);
        return true;
    }
    case AdminUsersRoles::Name:
    case AdminUsersRoles::UserName:
    case AdminUsersRoles::Email:
    case AdminUsersRoles::Roles:
    case AdminUsersRoles::Status:
    case AdminUsersRoles::UserId:
    case AdminUsersRoles::ActiveUserDisplay:
    case AdminUsersRoles::Administrator:
        return false;
    }
    return false;
}

QVariant AdminUsersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminUsersRoles>(section)) {
        case AdminUsersModel::Name:
            return i18n("Name");
        case AdminUsersModel::UserName:
            return i18n("UserName");
        case AdminUsersModel::Email:
            return i18n("Email");
        case AdminUsersModel::Roles:
            return i18n("Role");
        case AdminUsersModel::ActiveUserDisplay:
            return i18n("Disabled");
        case AdminUsersModel::Status:
            return i18n("Status");
        case AdminUsersModel::UserId:
        case AdminUsersModel::ActiveUser:
        case AdminUsersRoles::Administrator:
            return {};
        }
    }
    return {};
}

int AdminUsersModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(AdminUsersModel::LastColumn) + 1;
}

QList<int> AdminUsersModel::hideColumns() const
{
    return {AdminUsersRoles::UserId, AdminUsersRoles::ActiveUser, AdminUsersRoles::Administrator};
}

QVariant AdminUsersModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mUsers.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const User &user = mUsers.at(index.row());
    const int col = index.column();
    switch (static_cast<AdminUsersRoles>(col)) {
    case AdminUsersRoles::Name:
        return user.name();
    case AdminUsersRoles::UserName:
        return user.userName();
    case AdminUsersRoles::Email:
        return user.userEmailsInfo().email;
    case AdminUsersRoles::Roles:
        return user.i18nRoles().join(QLatin1Char(','));
    case AdminUsersRoles::Status:
        return Utils::displaytextFromPresenceStatus(user.status());
    case AdminUsersRoles::ActiveUserDisplay:
        return user.active() ? i18n("Active") : i18n("Disabled");
    case AdminUsersRoles::ActiveUser:
        return user.active();
    case AdminUsersRoles::UserId:
        return user.userId();
    case AdminUsersRoles::Administrator:
        return user.roles().contains(QStringLiteral("admin"));
    }
    return {};
}
