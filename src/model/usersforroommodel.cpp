/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "usersforroommodel.h"
#include "usersmodel.h"
#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

UsersForRoomModel::UsersForRoomModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

UsersForRoomModel::~UsersForRoomModel()
{
}

void UsersForRoomModel::insertUsers(const QVector<User> &users)
{
    mUsers.clear();
    beginInsertRows(QModelIndex(), 0, rowCount() - 1);
    mUsers = users;
    endInsertRows();
}

int UsersForRoomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mUsers.count();
}

QVariant UsersForRoomModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mUsers.count()) {
        return QVariant();
    }

    const User user = mUsers.at(index.row());
    switch (role) {
    case UserName:
        return user.userName();
    case IconStatus:
        return user.iconFromStatus();
    case UserId:
        return user.userId();
    case Name:
        return user.name();
    default:
        qCWarning(RUQOLA_LOG) << "Unknown usersmodel roles: " << role;
    }
    return {};
}

QHash<int, QByteArray> UsersForRoomModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UserName] = QByteArrayLiteral("username");
    roles[Name] = QByteArrayLiteral("name");
    roles[IconStatus] = QByteArrayLiteral("iconstatus");
    return roles;
}

void UsersForRoomModel::parseUsersForRooms(const QJsonObject &root)
{
    const QJsonObject result = root[QLatin1String("result")].toObject();
    if (!result.isEmpty()) {
        const QJsonArray records = result[QStringLiteral("records")].toArray();
        const int total = result[QLatin1String("total")].toInt();

        QVector<User> users;
        users.reserve(records.count());
        for (const QJsonValue &current : records) {
            if (current.type() == QJsonValue::Object) {
                const QJsonObject userObject = current.toObject();
                const QString userName = userObject[QStringLiteral("username")].toString();
                const QString name = userObject[QStringLiteral("name")].toString();
                const QString id = userObject[QStringLiteral("_id")].toString();
                User user;
                user.setName(name);
                user.setUserName(userName);
                user.setUserId(id);
                if (user.isValid()) {
                    users.append(user);
                } else {
                    qCWarning(RUQOLA_LOG) << "Invalid user" << user;
                }
            } else {
                qCWarning(RUQOLA_LOG) << "Parse records: Error in users for rooms json" << root;
            }
        }
        if (users.count() != total) {
            qCWarning(RUQOLA_LOG) << "Users for rooms, parsing error. Parse " << users.count() << " users but json give us a total number : "<< total;
        }
        insertUsers(users);
    } else {
        qCWarning(RUQOLA_LOG) << "Error in users for rooms json" << root;
    }
}

void UsersForRoomModel::userStatusChanged(const User &newuser)
{
    const int roomCount{
        mUsers.count()
    };
    for (int i = 0; i < roomCount; ++i) {
        User user = mUsers.at(i);
        if (newuser.userId() == user.userId()) {
            user.setStatus(newuser.status());
            mUsers.replace(i, user);
            const QModelIndex idx = createIndex(i, 0);
            Q_EMIT dataChanged(idx, idx);
        }
    }
}
