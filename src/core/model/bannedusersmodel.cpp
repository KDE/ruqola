/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannedusersmodel.h"
#include "rocketchataccount.h"

BannedUsersModel::BannedUsersModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mBannedUsers(new FileAttachments)
    , mRochetChantAccount(account)
{
}

BannedUsersModel::~BannedUsersModel()
{
    delete mBannedUsers;
}

void BannedUsersModel::checkFullList()
{
    setHasFullList(mBannedUsers->fileAttachments().count() == mBannedUsers->total());
}

bool BannedUsersModel::loadMoreFilesInProgress() const
{
    return mLoadMoreFilesInProgress;
}

void BannedUsersModel::setLoadMoreFilesInProgress(bool loadMoreFilesInProgress)
{
    if (mLoadMoreFilesInProgress != loadMoreFilesInProgress) {
        mLoadMoreFilesInProgress = loadMoreFilesInProgress;
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
    const int numberOfElement = mBannedUsers->fileAttachments().count();
    mBannedUsers->parseMoreFileAttachments(fileAttachmentsObj);
    beginInsertRows(QModelIndex(), numberOfElement, mBannedUsers->fileAttachments().count() - 1);
    endInsertRows();
    checkFullList();
}

void BannedUsersModel::initialize()
{
    mRoomId.clear();
    mLoadMoreFilesInProgress = false;
    setHasFullList(false);
}

void BannedUsersModel::parseBannedUsers(const QJsonObject &fileAttachmentsObj, const QString &roomId)
{
    mRoomId = roomId;
    if (rowCount() != 0) {
        clear();
    }
    mBannedUsers->parseFileAttachments(fileAttachmentsObj);
    if (!mBannedUsers->isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mBannedUsers->fileAttachments().count() - 1);
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

void BannedUsersModel::setFiles(const QList<File> &files)
{
    clear();
    if (!files.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, files.count() - 1);
        mBannedUsers->setFileAttachments(files);
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

    return mBannedUsers->fileAttachments().count();
}

QVariant BannedUsersModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mBannedUsers->fileAttachments().count()) {
        return {};
    }

    const File &file = mBannedUsers->fileAttachments()[index.row()];
    switch (role) {
        // TODO
    }
    return {};
}

FileAttachments *BannedUsersModel::fileAttachments() const
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
