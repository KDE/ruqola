/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

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
    const int numberOfElement = mFileAttachments->count();
    FileAttachments fileAttachments = *mFileAttachments;
    fileAttachments.parseMoreFileAttachments(fileAttachmentsObj);
    const int newNumberOfElement = fileAttachments.count();
    if (newNumberOfElement > numberOfElement) {
        beginInsertRows(QModelIndex(), numberOfElement, newNumberOfElement - 1);
        *mFileAttachments = std::move(fileAttachments);
        endInsertRows();
    } else { // No new element but offset/total may have changed
        *mFileAttachments = std::move(fileAttachments);
    }
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
    beginResetModel();
    mFileAttachments->clear();
    mFileAttachments->parseFileAttachments(fileAttachmentsObj);
    endResetModel();
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

void FilesForRoomModel::setFiles(QList<File> files)
{
    beginResetModel();
    mFileAttachments->setFileAttachments(std::move(files));
    endResetModel();
    checkFullList();
    Q_EMIT totalChanged();
}

int FilesForRoomModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) { // flat model
        return 0;
    }

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
