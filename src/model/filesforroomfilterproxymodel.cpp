/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

FilesForRoomFilterProxyModel::FilesForRoomFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(FilesForRoomModel::Url);
    setFilterRole(FilesForRoomModel::Url);
    sort(0);
}

FilesForRoomFilterProxyModel::~FilesForRoomFilterProxyModel()
{
}

QHash<int, QByteArray> FilesForRoomFilterProxyModel::roleNames() const
{
    if (QAbstractItemModel *source = sourceModel()) {
        return source->roleNames();
    }
    return QHash<int, QByteArray>();
}

bool FilesForRoomFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const QString leftString = sourceModel()->data(left, FilesForRoomModel::UserName).toString();
        const QString rightString = sourceModel()->data(right, FilesForRoomModel::UserName).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return false;
    }
}

void FilesForRoomFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}

int FilesForRoomFilterProxyModel::rowCount(const QModelIndex &parent) const
{
    if (QAbstractItemModel *source = sourceModel()) {
        qDebug() << " int FilesForRoomFilterProxyModel::rowCount(const QModelIndex &parent) const" << source->rowCount(parent);
        return source->rowCount(parent);
    }
    return 0;
}
