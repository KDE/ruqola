/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersmodel.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_debug.h"
#include "utils.h"
#include <KLocalizedString>
#include <QJsonObject>

UsersModel::UsersModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

UsersModel::~UsersModel() = default;

int UsersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) { // flat model
        return 0;
    }
    return mUsers.size();
}

QVariant UsersModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mUsers.count()) {
        return {};
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
    default:
        break;
    }

    return {};
}

bool UsersModel::userIsOffline(const QString &name) const
{
    const int userCount = mUsers.count();
    for (int i = 0; i < userCount; ++i) {
        const auto &user = mUsers.at(i);
        if (user.userName() == name) {
            return user.status() == User::PresenceStatus::Offline;
        }
    }
    return true;
}

QString UsersModel::userStatusStr(const QString &name) const
{
    const int userCount = mUsers.count();
    for (int i = 0; i < userCount; ++i) {
        const auto &user = mUsers.at(i);
        if (user.userName() == name) {
            return user.generateStatusStr();
        }
    }

    // qCWarning(RUQOLA_LOG) << "User for name " << name << " not defined yet";
    return i18n("Offline");
}

QString UsersModel::userStatusIconFileName(const QString &name) const
{
    const int userCount = mUsers.count();
    for (int i = 0; i < userCount; ++i) {
        const auto &user = mUsers.at(i);
        if (user.userName() == name) {
            return user.iconFromStatus();
        }
    }

    // qCWarning(RUQOLA_LOG) << "User for name " << name << " not defined yet";
    return u"user-offline"_s;
}

User::PresenceStatus UsersModel::status(const QByteArray &userId) const
{
    const int userCount = mUsers.count();

    for (int i = 0; i < userCount; ++i) {
        const auto &user = mUsers.at(i);
        if (user.userId() == userId) {
            return user.status();
        }
    }
    // Return offline as default;
    return User::PresenceStatus::Offline;
}

void UsersModel::removeUser(const QByteArray &userId)
{
    qCDebug(RUQOLA_LOG) << " User removed " << userId;
    const int userCount = mUsers.count();
    for (int i = 0; i < userCount; ++i) {
        if (mUsers.at(i).userId() == userId) {
            qCDebug(RUQOLA_LOG) << " User removed " << mUsers.at(i).name();
            // Send info as it's disconnected. But don't remove it from list
            User &user = mUsers[i];
            user.setStatus(User::PresenceStatus::Offline);
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
    const QByteArray id = array.value("id"_L1).toString().toLatin1();
    const int userCount = mUsers.count();
    for (int i = 0; i < userCount; ++i) {
        if (mUsers.at(i).userId() == id) {
            User &user = mUsers[i];
            const QJsonObject fields = array.value("fields"_L1).toObject();

            const QString newStatus = fields.value("status"_L1).toString();
            bool userDataChanged = false;
            if (!newStatus.isEmpty()) {
                user.setStatus(Utils::presenceStatusFromString(newStatus));
                const QModelIndex idx = createIndex(i, 0);
                Q_EMIT dataChanged(idx, idx);
                Q_EMIT userStatusChanged(user);
                userDataChanged = true;
            }
            const QString newName = fields.value("name"_L1).toString();
            if (!newName.isEmpty()) {
                user.setName(newName);
                const QModelIndex idx = createIndex(i, 0);
                Q_EMIT dataChanged(idx, idx);
                Q_EMIT userNameChanged(user);
                userDataChanged = true;
            }
            const QString newuserName = fields.value("username"_L1).toString();
            if (!newuserName.isEmpty()) {
                user.setUserName(newuserName);
                const QModelIndex idx = createIndex(i, 0);
                Q_EMIT dataChanged(idx, idx);
                Q_EMIT nameChanged(user);
                userDataChanged = true;
            }
            const QString statusMessage = fields.value("statusText"_L1).toString();
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

#include "moc_usersmodel.cpp"
