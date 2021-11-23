/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
