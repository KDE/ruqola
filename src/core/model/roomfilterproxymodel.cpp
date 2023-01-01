/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomfilterproxymodel.h"
#include "roommodel.h"

RoomFilterProxyModel::RoomFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    sort(0);
    setRecursiveFilteringEnabled(true);
}

RoomFilterProxyModel::~RoomFilterProxyModel() = default;

bool RoomFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    // assumes that we have a section → channels hierarchy
    if (left.parent().isValid() && right.parent().isValid()) {
        qint64 leftDate = 0;
        qint64 rightDate = 0;
        if (mSortOrder == OwnUserPreferences::RoomListSortOrder::ByLastMessage) {
            leftDate = sourceModel()->data(left, RoomModel::RoomLastMessageAt).toLongLong();
            rightDate = sourceModel()->data(right, RoomModel::RoomLastMessageAt).toLongLong();
        }
        if (leftDate == rightDate) {
            const QString leftString = sourceModel()->data(left, RoomModel::RoomFName).toString();
            const QString rightString = sourceModel()->data(right, RoomModel::RoomFName).toString();
            return QString::localeAwareCompare(leftString, rightString) < 0;
        } else {
            return leftDate > rightDate;
        }
    }
    return left.row() < right.row();
}

void RoomFilterProxyModel::setFilterString(const QString &string)
{
    mFilterString = string;
    invalidate();
}

void RoomFilterProxyModel::setSortOrder(OwnUserPreferences::RoomListSortOrder sortOrder)
{
    mSortOrder = sortOrder;
    invalidate();
}

bool RoomFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex modelIndex = sourceModel()->index(source_row, 0, source_parent);

    // By default don't display any sections
    // Thanks to recursive filtering, the sections with channels will be displayed
    if (!source_parent.isValid()) {
        return false;
    }

    auto match = [&](int role) {
        return mFilterString.isEmpty() || modelIndex.data(role).toString().contains(mFilterString, Qt::CaseInsensitive);
    };
    if (!match(RoomModel::RoomName) && !match(RoomModel::RoomFName)) {
        return false;
    }

    return sourceModel()->data(modelIndex, RoomModel::RoomOpen).toBool();
}
