/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelcompleterfilterproxymodel.h"
#include "channelcompletermodel.h"

ChannelCompleterFilterProxyModel::ChannelCompleterFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(ChannelCompleterModel::RoomName);
    sort(0);
}

ChannelCompleterFilterProxyModel::~ChannelCompleterFilterProxyModel() = default;

void ChannelCompleterFilterProxyModel::clear()
{
    static_cast<ChannelCompleterModel *>(sourceModel())->clear();
}

bool ChannelCompleterFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const QString leftString = sourceModel()->data(left, ChannelCompleterModel::RoomName).toString();
        const QString rightString = sourceModel()->data(right, ChannelCompleterModel::RoomName).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return false;
    }
}

#include "moc_channelcompleterfilterproxymodel.cpp"
