/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannedusersmodel.h"
#include "rocketchataccount.h"

BannedUsersModel::BannedUsersModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mBannedUsers(new BannedUsers)
    , mRochetChantAccount(account)
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

bool BannedUsersModel::loadMoreFilesInProgress() const
{
    return mLoadMoreBannedUsersInProgress;
}

void BannedUsersModel::setLoadMoreFilesInProgress(bool loadMoreFilesInProgress)
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

void BannedUsersModel::addMoreFileAttachments(const QJsonObject &fileAttachmentsObj)
{
    const int numberOfElement = mBannedUsers->bannedUsers().count();
    mBannedUsers->parseMoreBannedUsers(fileAttachmentsObj);
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

void BannedUsersModel::parseBannedUsers(const QJsonObject &fileAttachmentsObj, const QString &roomId)
{
    mRoomId = roomId;
    if (rowCount() != 0) {
        clear();
    }
    mBannedUsers->parseBannedUsers(fileAttachmentsObj);
    if (!mBannedUsers->isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mBannedUsers->bannedUsers().count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

QString BannedUsersModel::roomId() const
{
    return mRoomId;
}

void BannedUsersModel::setRoomId(const QString &roomId)
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

    const BannedUser &user = mBannedUsers->bannedUsers().at(index.row());
    switch (role) {
    case BannedUserRoles::Name:
        return user.name();
    case BannedUserRoles::UserName:
        return user.userName();
    case BannedUserRoles::Identifier:
        return user.identifier();
    }
    return {};
}

BannedUsers *BannedUsersModel::fileAttachments() const
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
