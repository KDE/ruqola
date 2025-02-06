/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersforroommodel.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_debug.h"
#include "usersmodel.h"
#include "utils.h"

#include <QIcon>
#include <QJsonArray>
#include <QJsonObject>

UsersForRoomModel::UsersForRoomModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

UsersForRoomModel::~UsersForRoomModel() = default;

void UsersForRoomModel::setUsers(const QList<User> &users)
{
    if (mUsers.isEmpty()) {
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
        beginResetModel();
        mUsers.clear();
        endResetModel();
    }
}

int UsersForRoomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mUsers.count();
}

QVariant UsersForRoomModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mUsers.count()) {
        return {};
    }

    const User &user = mUsers.at(index.row());
    switch (role) {
    case DisplayName:
        return generateDisplayName(user);
    case Qt::DisplayRole:
    case UserName:
        return user.userName();
    case IconStatus:
        return user.iconFromStatus();
    case UserId:
        return user.userId();
    case Name:
        return user.name();
    case Status:
        return Utils::presenceStatusToString(user.status()); // Translate it ?
    case AvatarInfo:
        return QVariant::fromValue(avatarInfo(user));
    case Qt::DecorationRole:
        return QIcon::fromTheme(user.iconFromStatus());
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

Utils::AvatarInfo UsersForRoomModel::avatarInfo(const User &user) const
{
    Utils::AvatarInfo info;
    info.avatarType = Utils::AvatarType::User;
    info.identifier = user.userName();
    return info;
}

void UsersForRoomModel::parseUsersForRooms(const QJsonObject &root, UsersModel *model, bool restapi)
{
    if (restapi) {
        mTotal = root["total"_L1].toInt();
        mOffset = root["offset"_L1].toInt();
        const QJsonArray members = root["members"_L1].toArray();
        QList<User> users;
        users.reserve(members.count());
        for (const QJsonValue &current : members) {
            if (current.type() == QJsonValue::Object) {
                const QJsonObject userObject = current.toObject();
                const QString userName = userObject["username"_L1].toString();
                const QString name = userObject["name"_L1].toString();
                const QByteArray id = userObject["_id"_L1].toString().toLatin1();
                const double utcOffset = userObject["utcOffset"_L1].toDouble();
                const QString status = userObject["status"_L1].toString();
                User user;
                user.setName(name);
                user.setUserName(userName);
                user.setUserId(id);
                user.setUtcOffset(utcOffset);
                user.setStatus(Utils::presenceStatusFromString(status));
                if (user.isValid()) {
                    users.append(std::move(user));
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
        const QJsonObject result = root["result"_L1].toObject();
        if (!result.isEmpty()) {
            const QJsonArray records = result["records"_L1].toArray();
            mTotal = result["total"_L1].toInt();
            mOffset = root["offset"_L1].toInt(); // TODO verify if a day we use no rest api

            QList<User> users;
            users.reserve(records.count());
            for (const QJsonValue &current : records) {
                if (current.type() == QJsonValue::Object) {
                    const QJsonObject userObject = current.toObject();
                    const QString userName = userObject["username"_L1].toString();
                    const QString name = userObject["name"_L1].toString();
                    const QByteArray id = userObject["_id"_L1].toString().toLatin1();
                    User user;
                    user.setName(name);
                    user.setUserName(userName);
                    user.setUserId(id);
                    if (model) {
                        user.setStatus(model->status(id));
                    }
                    // Add status!
                    if (user.isValid()) {
                        users.append(std::move(user));
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

void UsersForRoomModel::setUserStatusChanged(const User &newuser)
{
    const int roomCount = mUsers.count();
    for (int i = 0; i < roomCount; ++i) {
        User &user = mUsers[i];
        if (newuser.userId() == user.userId()) {
            user.setStatus(newuser.status());
            const QModelIndex idx = createIndex(i, 0);
            Q_EMIT dataChanged(idx, idx);
            Q_EMIT userStatusChanged(user.userId());
            break;
        }
    }
}

void UsersForRoomModel::setLoadMoreUsersInProgress(bool inProgress)
{
    if (mLoadingInProgress != inProgress) {
        mLoadingInProgress = inProgress;
        Q_EMIT loadingInProgressChanged();
    }
}

bool UsersForRoomModel::loadMoreUsersInProgress() const
{
    return mLoadingInProgress;
}

#include "moc_usersforroommodel.cpp"
