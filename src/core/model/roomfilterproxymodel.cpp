/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomfilterproxymodel.h"
#include "roommodel.h"

RoomFilterProxyModel::RoomFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    sort(0);
}

RoomFilterProxyModel::~RoomFilterProxyModel() = default;

bool RoomFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const int orderLeftData = sourceModel()->data(left, RoomModel::RoomOrder).toInt();
        const int orderRightData = sourceModel()->data(right, RoomModel::RoomOrder).toInt();
        if (orderLeftData == orderRightData) {
            const QString leftString = sourceModel()->data(left, RoomModel::RoomFName).toString();
            const QString rightString = sourceModel()->data(right, RoomModel::RoomFName).toString();
            return QString::localeAwareCompare(leftString, rightString) < 0;
        } else {
            return orderLeftData < orderRightData;
        }
    }
    return false;
}

void RoomFilterProxyModel::setFilterString(const QString &string)
{
    mFilterString = string;
    invalidate();
}

bool RoomFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex modelIndex = sourceModel()->index(source_row, 0, source_parent);

    auto match = [&](int role) {
        return mFilterString.isEmpty() || modelIndex.data(role).toString().contains(mFilterString, Qt::CaseInsensitive);
    };
    if (!match(RoomModel::RoomName) && !match(RoomModel::RoomFName)) {
        return false;
    }

    return sourceModel()->data(modelIndex, RoomModel::RoomOpen).toBool();
}
