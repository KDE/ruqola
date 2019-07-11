/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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

void UsersForRoomModel::removeUser(const QString &userId)
{
    //TODO verify if it
}

void UsersForRoomModel::addUser(const User &users)
{
    //TODO verify if it
}

void UsersForRoomModel::setUsers(const QVector<User> &users)
{
    if (mUsers.isEmpty()) {
        if (rowCount() != 0) {
            beginRemoveRows(QModelIndex(), 0, mUsers.count() - 1);
            mUsers.clear();
            endRemoveRows();
        }
        if (!users.isEmpty()) {
            beginInsertRows(QModelIndex(), 0, users.count() - 1);
            mUsers = users;
            endInsertRows();
        }
    } else {
        const int numberOfElement = mUsers.count();
        mUsers << users;
        beginInsertRows(QModelIndex(), numberOfElement, mUsers.count() - 1);
        endInsertRows();
    }
    checkFullList();
}

void UsersForRoomModel::clear()
{
    if (!mUsers.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, mUsers.count() - 1);
        mUsers.clear();
        endRemoveRows();
    }
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
    case DisplayName:
        return generateDisplayName(user);
    case UserName:
        return user.userName();
    case IconStatus:
        return user.iconFromStatus();
    case UserId:
        return user.userId();
    case Name:
        return user.name();
    }

    return {};
}

QString UsersForRoomModel::generateDisplayName(const User &user) const
{
    const QString displayName = QStringLiteral("<a href=\'%1\'>%1</a>").arg(user.userName().isEmpty() ? user.name() : user.userName());
    return displayName;
}

void UsersForRoomModel::checkFullList()
{
    setHasFullList(mUsers.count() == mTotal);
}

bool UsersForRoomModel::hasFullList() const
{
    return mHasFullList;
}

void UsersForRoomModel::setHasFullList(bool hasFullList)
{
    if (mHasFullList != hasFullList) {
        mHasFullList = hasFullList;
        Q_EMIT hasFullListChanged();
    }
}

int UsersForRoomModel::usersCount() const
{
    return mUsers.count();
}

int UsersForRoomModel::offset() const
{
    return mOffset;
}

void UsersForRoomModel::setOffset(int offset)
{
    mOffset = offset;
}

int UsersForRoomModel::total() const
{
    return mTotal;
}

void UsersForRoomModel::setTotal(int total)
{
    mTotal = total;
}

QHash<int, QByteArray> UsersForRoomModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UserName] = QByteArrayLiteral("username");
    roles[Name] = QByteArrayLiteral("name");
    roles[UserId] = QByteArrayLiteral("userid");
    roles[IconStatus] = QByteArrayLiteral("iconstatus");
    roles[DisplayName] = QByteArrayLiteral("displayname");
    return roles;
}

void UsersForRoomModel::parseUsersForRooms(const QJsonObject &root, UsersModel *model, bool restapi)
{
    if (restapi) {
        mTotal = root[QLatin1String("total")].toInt();
        mOffset = root[QLatin1String("offset")].toInt();
        const QJsonArray members = root[QStringLiteral("members")].toArray();
        QVector<User> users;
        users.reserve(members.count());
        for (const QJsonValue &current : members) {
            if (current.type() == QJsonValue::Object) {
                const QJsonObject userObject = current.toObject();
                const QString userName = userObject[QStringLiteral("username")].toString();
                const QString name = userObject[QStringLiteral("name")].toString();
                const QString id = userObject[QStringLiteral("_id")].toString();
                const int utcOffset = userObject[QStringLiteral("utcOffset")].toInt();
                const QString status = userObject[QStringLiteral("status")].toString();
                User user;
                user.setName(name);
                user.setUserName(userName);
                user.setUserId(id);
                user.setUtcOffset(utcOffset);
                user.setStatus(status);
                if (user.isValid()) {
                    users.append(user);
                } else {
                    qCWarning(RUQOLA_LOG) << "Invalid user" << user;
                    mTotal--;
                }
            } else {
                qCWarning(RUQOLA_LOG) << "Parse records: Error in users for rooms json" << root;
            }
        }
        setUsers(users);
    } else {
        const QJsonObject result = root[QLatin1String("result")].toObject();
        if (!result.isEmpty()) {
            const QJsonArray records = result[QStringLiteral("records")].toArray();
            mTotal = result[QLatin1String("total")].toInt();
            mOffset = root[QLatin1String("offset")].toInt(); //TODO verify if a day we use no rest api

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
                    if (model) {
                        user.setStatus(model->status(id));
                    }
                    //Add status!
                    if (user.isValid()) {
                        users.append(user);
                    } else {
                        qCWarning(RUQOLA_LOG) << "Invalid user" << user;
                        mTotal--;
                    }
                } else {
                    qCWarning(RUQOLA_LOG) << "Parse records: Error in users for rooms json" << root;
                }
            }
            setUsers(users);
        } else {
            qCWarning(RUQOLA_LOG) << "Error in users for rooms json" << root;
        }
    }
}

void UsersForRoomModel::userStatusChanged(const User &newuser)
{
    const int roomCount = mUsers.count();
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
