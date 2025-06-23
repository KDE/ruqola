/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersforroommodel.h"

#include "ruqola_debug.h"
#include "usersmodel.h"

#include <KLocalizedString>

#include <QIcon>
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
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
        for (const auto &u : users) {
            if (!mUsers.contains(u)) {
                mUsers << u;
            }
        }
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
    if (parent.isValid()) // flat model
        return 0;
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
        return QVariant::fromValue(user.status());
    case StatusStr:
        return Utils::presenceStatusToString(user.status()); // Translate it ?
    case AvatarInfo:
        return QVariant::fromValue(avatarInfo(user));
    case Roles:
        return user.roles();
    case Qt::DecorationRole:
        return QIcon::fromTheme(user.iconFromStatus());
    case Section:
        return QVariant::fromValue(section(user));
    }

    return {};
}

UsersForRoomModel::SectionStatus UsersForRoomModel::section(const User &user) const
{
    const QStringList roles = user.roles();
    if (roles.contains(QStringLiteral("owner"))) {
        return UsersForRoomModel::SectionStatus::Owner;
    }
    switch (user.status()) {
    case User::PresenceStatus::Online:
        return UsersForRoomModel::SectionStatus::Online;
    case User::PresenceStatus::Busy:
        return UsersForRoomModel::SectionStatus::Busy;
    case User::PresenceStatus::Away:
        return UsersForRoomModel::SectionStatus::Away;
    case User::PresenceStatus::Offline:
        return UsersForRoomModel::SectionStatus::Offline;
    case User::PresenceStatus::Unknown:
        return UsersForRoomModel::SectionStatus::Unknown;
    }
    return UsersForRoomModel::SectionStatus::Unknown;
}

int UsersForRoomModel::numberUsersWithoutFilter() const
{
    return mNumberUsersWithoutFilter;
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

void UsersForRoomModel::parseUsersForRooms(const QJsonObject &root, UsersModel *model, bool restapi, const QString &filter)
{
    if (restapi) {
        if (filter.isEmpty()) {
            mTotal = root["total"_L1].toInt();
            mOffset = root["offset"_L1].toInt();
        }
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
                const QJsonArray rolesArray = userObject["roles"_L1].toArray();
                QStringList roles;
                const int total = rolesArray.size();
                roles.reserve(total);
                for (int i = 0; i < total; ++i) {
                    roles.append(rolesArray.at(i).toString());
                }
                User user;
                user.setName(name);
                user.setUserName(userName);
                user.setUserId(id);
                user.setUtcOffset(utcOffset);
                user.setStatus(Utils::presenceStatusFromString(status));
                user.setRoles(roles, {}); // TODO add roleInfo
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
        if (filter.isEmpty()) {
            mNumberUsersWithoutFilter += users.count();
        }
    } else {
        const QJsonObject result = root["result"_L1].toObject();
        if (!result.isEmpty()) {
            const QJsonArray records = result["records"_L1].toArray();
            if (filter.isEmpty()) {
                mTotal = result["total"_L1].toInt();
                mOffset = root["offset"_L1].toInt(); // TODO verify if a day we use no rest api
            }

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

QString UsersForRoomModel::sectionName(UsersForRoomModel::SectionStatus sectionId)
{
    switch (sectionId) {
    case UsersForRoomModel::SectionStatus::Owner:
        return i18n("Owner");
    case UsersForRoomModel::SectionStatus::Online:
        return Utils::i18nFromPresenceStatus(User::PresenceStatus::Online);
    case UsersForRoomModel::SectionStatus::Busy:
        return Utils::i18nFromPresenceStatus(User::PresenceStatus::Busy);
    case UsersForRoomModel::SectionStatus::Away:
        return Utils::i18nFromPresenceStatus(User::PresenceStatus::Away);
    case UsersForRoomModel::SectionStatus::Offline:
        return Utils::i18nFromPresenceStatus(User::PresenceStatus::Offline);
    case UsersForRoomModel::SectionStatus::Unknown:
        return i18n("Unknown");
    case UsersForRoomModel::SectionStatus::NSections:
        break;
    }
    return QStringLiteral("ERROR");
}

#include "moc_usersforroommodel.cpp"
