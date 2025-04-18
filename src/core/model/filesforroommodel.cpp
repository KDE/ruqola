/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filesforroommodel.h"
#include "rocketchataccount.h"

FilesForRoomModel::FilesForRoomModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mFileAttachments(new FileAttachments)
    , mRochetChantAccount(account)
{
}

FilesForRoomModel::~FilesForRoomModel()
{
    delete mFileAttachments;
}

void FilesForRoomModel::checkFullList()
{
    setHasFullList(mFileAttachments->fileAttachments().count() == mFileAttachments->total());
}

bool FilesForRoomModel::loadMoreFilesInProgress() const
{
    return mLoadMoreFilesInProgress;
}

void FilesForRoomModel::setLoadMoreFilesInProgress(bool loadMoreFilesInProgress)
{
    if (mLoadMoreFilesInProgress != loadMoreFilesInProgress) {
        mLoadMoreFilesInProgress = loadMoreFilesInProgress;
        Q_EMIT loadingInProgressChanged();
    }
}

void FilesForRoomModel::clear()
{
    beginResetModel();
    mFileAttachments->clear();
    endResetModel();
}

void FilesForRoomModel::addMoreFileAttachments(const QJsonObject &fileAttachmentsObj)
{
    const int numberOfElement = mFileAttachments->fileAttachments().count();
    mFileAttachments->parseMoreFileAttachments(fileAttachmentsObj);
    beginInsertRows(QModelIndex(), numberOfElement, mFileAttachments->fileAttachments().count() - 1);
    endInsertRows();
    checkFullList();
}

void FilesForRoomModel::initialize()
{
    mRoomId.clear();
    mLoadMoreFilesInProgress = false;
    setHasFullList(false);
}

void FilesForRoomModel::parseFileAttachments(const QJsonObject &fileAttachmentsObj, const QString &roomId)
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

QString FilesForRoomModel::roomId() const
{
    return mRoomId;
}

void FilesForRoomModel::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

void FilesForRoomModel::setFiles(const QList<File> &files)
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

int FilesForRoomModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) // flat model
        return 0;

    return mFileAttachments->fileAttachments().count();
}

QVariant FilesForRoomModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mFileAttachments->fileAttachments().count()) {
        return {};
    }

    const File &file = mFileAttachments->fileAttachments()[index.row()];
    switch (role) {
    case FilePointer:
        return QVariant::fromValue(&file);
    case FileName:
        return file.fileName();
    case UserId:
        return file.userId();
    case MimeType:
        return file.mimeType();
    case Url:
        return file.url();
    case Description:
        return file.description();
    case CanBeDeleted:
        return mRochetChantAccount->userId() == file.userId();
    case FileId:
        return file.fileId();
    case TimeStamp:
        return file.uploadedDateTimeStr();
    case UserName:
        return file.userName();
    case SortByTimeStamp:
        return file.uploadedAt();
    case Complete:
        return file.complete();
    case TypeGroup:
        return file.typeGroup();
    }
    return {};
}

FileAttachments *FilesForRoomModel::fileAttachments() const
{
    return mFileAttachments;
}

int FilesForRoomModel::total() const
{
    if (mFileAttachments) {
        return mFileAttachments->total();
    }
    return -1;
}

void FilesForRoomModel::setHasFullList(bool state)
{
    if (mHasFullList != state) {
        mHasFullList = state;
        Q_EMIT hasFullListChanged();
    }
}

bool FilesForRoomModel::hasFullList() const
{
    return mHasFullList;
}

#include "moc_filesforroommodel.cpp"
