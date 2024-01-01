/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelcompleterfilterproxymodel.h"
#include "inputcompletermodel.h"

ChannelCompleterFilterProxyModel::ChannelCompleterFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(InputCompleterModel::CompleterName);
    sort(0);
}

ChannelCompleterFilterProxyModel::~ChannelCompleterFilterProxyModel() = default;

void ChannelCompleterFilterProxyModel::clear()
{
    static_cast<InputCompleterModel *>(sourceModel())->clear();
}

bool ChannelCompleterFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const QString leftString = sourceModel()->data(left, InputCompleterModel::CompleterName).toString();
        const QString rightString = sourceModel()->data(right, InputCompleterModel::CompleterName).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return false;
    }
}

#include "moc_channelcompleterfilterproxymodel.cpp"
