/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deviceinfoproxymodel.h"
#include "deviceinfomodel.h"

DeviceInfoProxyModel::DeviceInfoProxyModel(QAbstractListModel *model, QObject *parent)
    : SearchTreeBaseFilterProxyModel(model, parent)
{
}

DeviceInfoProxyModel::~DeviceInfoProxyModel() = default;

bool DeviceInfoProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const int leftColumn{left.column()};
    if (leftColumn == DeviceInfoModel::LoginAtDisplay) {
        const QModelIndex leftMessageModelIndex = sourceModel()->index(left.row(), DeviceInfoModel::LoginAt);
        const QModelIndex rightMessageModelIndex = sourceModel()->index(right.row(), DeviceInfoModel::LoginAt);
        return SearchTreeBaseFilterProxyModel::lessThan(leftMessageModelIndex, rightMessageModelIndex);
    }
    return SearchTreeBaseFilterProxyModel::lessThan(left, right);
}

#include "moc_deviceinfoproxymodel.cpp"
