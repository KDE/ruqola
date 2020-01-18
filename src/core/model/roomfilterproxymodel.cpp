/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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

#include "roomfilterproxymodel.h"
#include "roommodel.h"

RoomFilterProxyModel::RoomFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(RoomModel::RoomName);
    sort(0);
}

RoomFilterProxyModel::~RoomFilterProxyModel()
{
}

bool RoomFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const int orderLeftData = sourceModel()->data(left, RoomModel::RoomOrder).toInt();
        const int orderRightData = sourceModel()->data(right, RoomModel::RoomOrder).toInt();
        if (orderLeftData == orderRightData) {
            const QString leftString = sourceModel()->data(left, RoomModel::RoomName).toString();
            const QString rightString = sourceModel()->data(right, RoomModel::RoomName).toString();
            return QString::localeAwareCompare(leftString, rightString) < 0;
        } else {
            return orderLeftData < orderRightData;
        }
    }
    return false;
}

QHash<int, QByteArray> RoomFilterProxyModel::roleNames() const
{
    if (QAbstractItemModel *source = sourceModel()) {
        return source->roleNames();
    }
    return QHash<int, QByteArray>();
}

void RoomFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}


bool RoomFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex modelIndex = sourceModel()->index(source_row, 0, source_parent);
    return sourceModel()->data(modelIndex, RoomModel::RoomOpen).toBool();
}
