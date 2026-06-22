/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannedusersmodel.h"

BannedUsersModel::BannedUsersModel(QObject *parent)
    : QAbstractListModel(parent)
    , mBannedUsers(new BannedUsers)
{
}

BannedUsersModel::~BannedUsersModel()
{
    delete mBannedUsers;
}

void BannedUsersModel::checkFullList()
{
    setHasFullList(mBannedUsers->bannedUsers().count() == mBannedUsers->total());
}

bool BannedUsersModel::loadMoreBannedUsersInProgress() const
{
    return mLoadMoreBannedUsersInProgress;
}

void BannedUsersModel::setLoadMoreBannedUsersInProgress(bool loadMoreFilesInProgress)
{
    if (mLoadMoreBannedUsersInProgress != loadMoreFilesInProgress) {
        mLoadMoreBannedUsersInProgress = loadMoreFilesInProgress;
        Q_EMIT loadingInProgressChanged();
    }
}

void BannedUsersModel::clear()
{
    beginResetModel();
    mBannedUsers->clear();
    endResetModel();
}

void BannedUsersModel::removeBannedUsers(const QString &userName)
{
    const int roomCount = mBannedUsers->count();
    for (int i = 0; i < roomCount; ++i) {
        if (mBannedUsers->at(i).userName() == userName) {
            beginRemoveRows(QModelIndex(), i, i);
            mBannedUsers->takeAt(i);
            endRemoveRows();
            break;
        }
    }
}

void BannedUsersModel::addMoreBannedUsers(const QJsonObject &bannedUsersObj)
{
    const int numberOfElement = mBannedUsers->bannedUsers().count();
    mBannedUsers->parseMoreBannedUsers(bannedUsersObj);
    beginInsertRows(QModelIndex(), numberOfElement, mBannedUsers->bannedUsers().count() - 1);
    endInsertRows();
    checkFullList();
}

void BannedUsersModel::initialize()
{
    mRoomId.clear();
    mLoadMoreBannedUsersInProgress = false;
    setHasFullList(false);
}

void BannedUsersModel::parseBannedUsers(const QJsonObject &bannedUsersObj, const QByteArray &roomId)
{
    mRoomId = roomId;
    if (rowCount() != 0) {
        clear();
    }
    mBannedUsers->parseBannedUsers(bannedUsersObj);
    if (!mBannedUsers->isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mBannedUsers->bannedUsers().count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

QByteArray BannedUsersModel::roomId() const
{
    return mRoomId;
}

void BannedUsersModel::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

void BannedUsersModel::setBannedUsers(const QList<BannedUser> &users)
{
    clear();
    if (!users.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, users.count() - 1);
        mBannedUsers->setBannedUsers(users);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

int BannedUsersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) { // flat model
        return 0;
    }

    return mBannedUsers->bannedUsers().count();
}

QVariant BannedUsersModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mBannedUsers->bannedUsers().count()) {
        return {};
    }

    const BannedUser user = mBannedUsers->bannedUsers().at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case BannedUserRoles::Name:
        return user.name();
    case BannedUserRoles::UserName:
        return user.userName();
    case BannedUserRoles::Identifier:
        return user.identifier();
    }
    return {};
}

BannedUsers *BannedUsersModel::bannedUsers() const
{
    return mBannedUsers;
}

int BannedUsersModel::total() const
{
    if (mBannedUsers) {
        return mBannedUsers->total();
    }
    return -1;
}

void BannedUsersModel::setHasFullList(bool state)
{
    if (mHasFullList != state) {
        mHasFullList = state;
        Q_EMIT hasFullListChanged();
    }
}

bool BannedUsersModel::hasFullList() const
{
    return mHasFullList;
}

#include "moc_bannedusersmodel.cpp"
