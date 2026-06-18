/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannedusersmodel.h"
#include "rocketchataccount.h"

BannedUsersModel::BannedUsersModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mFileAttachments(new FileAttachments)
    , mRochetChantAccount(account)
{
}

BannedUsersModel::~BannedUsersModel()
{
    delete mFileAttachments;
}

void BannedUsersModel::checkFullList()
{
    setHasFullList(mFileAttachments->fileAttachments().count() == mFileAttachments->total());
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
    mFileAttachments->clear();
    endResetModel();
}

void BannedUsersModel::addMoreFileAttachments(const QJsonObject &fileAttachmentsObj)
{
    const int numberOfElement = mFileAttachments->fileAttachments().count();
    mFileAttachments->parseMoreFileAttachments(fileAttachmentsObj);
    beginInsertRows(QModelIndex(), numberOfElement, mFileAttachments->fileAttachments().count() - 1);
    endInsertRows();
    checkFullList();
}

void BannedUsersModel::initialize()
{
    mRoomId.clear();
    mLoadMoreFilesInProgress = false;
    setHasFullList(false);
}

void BannedUsersModel::parseFileAttachments(const QJsonObject &fileAttachmentsObj, const QString &roomId)
{
    mRoomId = roomId;
    if (rowCount() != 0) {
        clear();
    }
    mFileAttachments->parseFileAttachments(fileAttachmentsObj);
    if (!mFileAttachments->isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mFileAttachments->fileAttachments().count() - 1);
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
        mFileAttachments->setFileAttachments(files);
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

    return mFileAttachments->fileAttachments().count();
}

QVariant BannedUsersModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mFileAttachments->fileAttachments().count()) {
        return {};
    }

    const File &file = mFileAttachments->fileAttachments()[index.row()];
    switch (role) {
        // TODO
    }
    return {};
}

FileAttachments *BannedUsersModel::fileAttachments() const
{
    return mFileAttachments;
}

int BannedUsersModel::total() const
{
    if (mFileAttachments) {
        return mFileAttachments->total();
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
