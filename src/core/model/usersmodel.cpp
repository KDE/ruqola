/*
   Copyright (c) 2017-2021 Laurent Montel <montel@kde.org>

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

#include "usersmodel.h"
#include "ruqola_debug.h"
#include "utils.h"
#include <QJsonObject>

UsersModel::UsersModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

UsersModel::~UsersModel()
{
}

int UsersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mUsers.size();
}

QVariant UsersModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mUsers.count()) {
        return QVariant();
    }
    const User &user = mUsers.at(index.row());
    switch (role) {
    case UserName:
        return user.name();
    case UserId:
        return user.userId();
    case UserStatus:
        return Utils::presenceStatusToString(user.status());
    case UserIcon:
        return user.iconFromStatus();
    case UserStatusText:
        return user.statusText();
    }

    return {};
}

QString UsersModel::userStatusIconFileName(const QString &name) const
{
    const int userCount = mUsers.count();
    for (int i = 0; i < userCount; ++i) {
        if (mUsers.at(i).userName() == name) {
            return mUsers.at(i).iconFromStatus();
        }
    }

    // qCWarning(RUQOLA_LOG) << "User for name " << name << " not defined yet";
    return QStringLiteral("user-offline");
}

User::PresenceStatus UsersModel::status(const QString &userId) const
{
    const int userCount = mUsers.count();

    for (int i = 0; i < userCount; ++i) {
        if (mUsers.at(i).userId() == userId) {
            return mUsers.at(i).status();
        }
    }
    // Return offline as default;
    return User::PresenceStatus::PresenceOffline;
}

User UsersModel::fullUserInfo(const QString &userName) const
{
    const int userCount = mUsers.count();

    for (int i = 0; i < userCount; ++i) {
        const User user = mUsers.at(i);
        if (user.userName() == userName) {
            return user;
        }
    }
    return {};
}

void UsersModel::removeUser(const QString &userId)
{
    qCDebug(RUQOLA_LOG) << " User removed " << userId;
    const int userCount = mUsers.count();
    for (int i = 0; i < userCount; ++i) {
        if (mUsers.at(i).userId() == userId) {
            qCDebug(RUQOLA_LOG) << " User removed " << mUsers.at(i).name();
            // Send info as it's disconnected. But don't remove it from list
            User &user = mUsers[i];
            user.setStatus(User::PresenceStatus::PresenceOffline);
            const QModelIndex idx = createIndex(i, 0);
            Q_EMIT userStatusChanged(user);
            Q_EMIT dataChanged(idx, idx);
            break;
        }
    }
}

void UsersModel::addUser(const User &newuser)
{
    // It can be duplicate as we don't remove user from list when user is disconnected. Otherwise it will not sync with
    // user for room list
    qCDebug(RUQOLA_LOG) << " User added " << newuser;
    const int userCount = mUsers.count();
    bool found = false;
    for (int i = 0; i < userCount; ++i) {
        if (mUsers.at(i).userId() == newuser.userId()) {
            found = true;
            User &user = mUsers[i];
            user.setStatus(newuser.status());
            const QModelIndex idx = createIndex(i, 0);
            Q_EMIT userStatusChanged(user);
            Q_EMIT dataChanged(idx, idx);
            break;
        }
    }
    if (!found) {
        const int pos = mUsers.size();
        beginInsertRows(QModelIndex(), pos, pos);
        mUsers.append(newuser);
        endInsertRows();
    }
}

void UsersModel::updateUser(const QJsonObject &array)
{
    const QString id = array.value(QLatin1String("id")).toString();
    const int userCount = mUsers.count();
    for (int i = 0; i < userCount; ++i) {
        if (mUsers.at(i).userId() == id) {
            User &user = mUsers[i];
            const QJsonObject fields = array.value(QLatin1String("fields")).toObject();

            const QString newStatus = fields.value(QLatin1String("status")).toString();
            bool userDataChanged = false;
            if (!newStatus.isEmpty()) {
                user.setStatus(Utils::presenceStatusFromString(newStatus));
                const QModelIndex idx = createIndex(i, 0);
                Q_EMIT dataChanged(idx, idx);
                Q_EMIT userStatusChanged(user);
                userDataChanged = true;
            }
            const QString newName = fields.value(QLatin1String("name")).toString();
            if (!newName.isEmpty()) {
                user.setName(newName);
                const QModelIndex idx = createIndex(i, 0);
                Q_EMIT dataChanged(idx, idx);
                Q_EMIT userNameChanged(user);
                userDataChanged = true;
            }
            const QString newuserName = fields.value(QLatin1String("username")).toString();
            if (!newuserName.isEmpty()) {
                user.setUserName(newuserName);
                const QModelIndex idx = createIndex(i, 0);
                Q_EMIT dataChanged(idx, idx);
                Q_EMIT nameChanged(user);
                userDataChanged = true;
            }
            const QString statusMessage = fields.value(QLatin1String("statusText")).toString();
            if (!statusMessage.isEmpty()) {
                user.setStatusText(statusMessage);
                const QModelIndex idx = createIndex(i, 0);
                Q_EMIT dataChanged(idx, idx);
                Q_EMIT statusMessageChanged(user);
                userDataChanged = true;
            }
            if (!userDataChanged) {
                qCWarning(RUQOLA_LOG) << " Unsupported yet user data modification " << array;
            }
            break;
        }
    }
}
