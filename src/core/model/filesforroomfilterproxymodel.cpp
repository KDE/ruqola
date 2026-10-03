/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filesforroomfilterproxymodel.h"
#include "filesforroommodel.h"

FilesForRoomFilterProxyModel::FilesForRoomFilterProxyModel(FilesForRoomModel *fileModel, QObject *parent)
    : SortFilterProxyModelBase(parent)
    , mFilesForRoomModel(fileModel)
{
    setSourceModel(mFilesForRoomModel);

    setFilterCaseSensitivity(Qt::CaseInsensitive);
    // Filter on filename for the moment
    setFilterRole(FilesForRoomModel::FileName);

    setSortRole(FilesForRoomModel::SortByTimeStamp);
    sort(0, Qt::DescendingOrder);
    connect(mFilesForRoomModel, &FilesForRoomModel::hasFullListChanged, this, &FilesForRoomFilterProxyModel::hasFullListChanged);
    connect(mFilesForRoomModel, &FilesForRoomModel::totalChanged, this, &FilesForRoomFilterProxyModel::totalChanged);
    connect(mFilesForRoomModel, &FilesForRoomModel::loadingInProgressChanged, this, &FilesForRoomFilterProxyModel::loadingInProgressChanged);
}

FilesForRoomFilterProxyModel::~FilesForRoomFilterProxyModel() = default;

int FilesForRoomFilterProxyModel::total() const
{
    return mFilesForRoomModel->total();
}

bool FilesForRoomFilterProxyModel::hasFullList() const
{
    return mFilesForRoomModel->hasFullList();
}

int FilesForRoomFilterProxyModel::attachmentCount() const
{
    return mFilesForRoomModel->rowCount();
}

void FilesForRoomFilterProxyModel::resetTypeGroup()
{
    setTypeGroup({});
}

void FilesForRoomFilterProxyModel::clear()
{
    mFilesForRoomModel->clear();
}

void FilesForRoomFilterProxyModel::setTypeGroup(const QString &typeGroup)
{
    if (mTypeGroup != typeGroup) {
        beginFilterChange();
        mTypeGroup = typeGroup;
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
    }
}

bool FilesForRoomFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mFilterString.isEmpty() && mTypeGroup.isEmpty()) {
        return true;
    } else {
        const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
        const QString fileName = sourceIndex.data(FilesForRoomModel::FileName).toString();
        const QString username = sourceIndex.data(FilesForRoomModel::UserName).toString();
        const bool indexContains = contains(fileName) || contains(username);
        if (!mFilterString.isEmpty() && mTypeGroup.isEmpty()) {
            return indexContains;
        }
        if (!mTypeGroup.isEmpty()) {
            const QString typegroup = sourceIndex.data(FilesForRoomModel::TypeGroup).toString();
            return (mTypeGroup == typegroup) && indexContains;
        }
    }
    return false;
}

#include "moc_filesforroomfilterproxymodel.cpp"
