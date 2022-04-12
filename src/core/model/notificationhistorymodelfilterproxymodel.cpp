/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorymodelfilterproxymodel.h"

NotificationHistoryModelFilterProxyModel::NotificationHistoryModelFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

NotificationHistoryModelFilterProxyModel::~NotificationHistoryModelFilterProxyModel() = default;

bool NotificationHistoryModelFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    //    if (!mCurrentCategory.isEmpty()) {
    //        const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    //        const QString category = sourceIndex.data(EmoticonModel::Category).toString();
    //        return mCurrentCategory == category;
    //    }
    return true;
}
