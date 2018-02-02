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

#include "usersmodelforroom.h"
#include "usersmodel.h"
#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

UsersModelForRoom::UsersModelForRoom(QObject *parent)
    : QAbstractListModel(parent)
{
}

UsersModelForRoom::~UsersModelForRoom()
{
}

void UsersModelForRoom::insertUsers(const QVector<User> &users)
{
    mUsers.clear();
    beginInsertRows(QModelIndex(), 0, users.count());
    mUsers = users;
    endInsertRows();
}

int UsersModelForRoom::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mUsers.count();
}

QVariant UsersModelForRoom::data(const QModelIndex &index, int role) const
{
    const User user = mUsers.at(index.row());
    switch (role) {
    case UserName:
        return user.userName();
    case UserIconStatus:
        return user.iconFromStatus();
    default:
        qCWarning(RUQOLA_LOG) << "Unknown usersmodel roles: " << role;
    }
    return {};
}

QHash<int, QByteArray> UsersModelForRoom::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UserName] = QByteArrayLiteral("username");
    roles[UserIconStatus] = QByteArrayLiteral("iconstatus");
    return roles;
}


void UsersModelForRoom::parseUsersForRooms(const QJsonObject &root)
{
    const QJsonObject result = root[QLatin1String("result")].toObject();
    if (!result.isEmpty()) {
        const QJsonArray records = result[QStringLiteral("records")].toArray();
        const int total = result[QLatin1String("total")].toInt();

        QVector<User> users;
        users.reserve(records.count());
        for ( const QJsonValue &current : records ) {
            if ( current.type() == QJsonValue::Object ) {
                const QJsonObject userObject = current.toObject();
                const QString userName = userObject[QStringLiteral("username")].toString();
                const QString name = userObject[QStringLiteral("name")].toString();
                const QString id = userObject[QStringLiteral("_id")].toString();
                User user;
                user.setName(name);
                user.setUserName(userName);
                user.setUserId(id);
                users.append(user);
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

void UsersModelForRoom::userStatusChanged(const User &newuser)
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
