/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filesforroomfilterproxymodel.h"
#include "filesforroommodel.h"

FilesForRoomFilterProxyModel::FilesForRoomFilterProxyModel(FilesForRoomModel *fileModel, QObject *parent)
    : QSortFilterProxyModel(parent)
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

void FilesForRoomFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}

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

void FilesForRoomFilterProxyModel::setTypeGroup(const QString &typeGroup)
{
    if (mTypeGroup != typeGroup) {
        mTypeGroup = typeGroup;
        invalidateFilter();
    }
}

bool FilesForRoomFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mTypeGroup.isEmpty()) {
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    const QString typegroup = sourceIndex.data(FilesForRoomModel::TypeGroup).toString();
    return (mTypeGroup == typegroup) && QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

#include "moc_filesforroomfilterproxymodel.cpp"
