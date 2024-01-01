/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonfilterproxymodel.h"
#include "emoticonmodel.h"

EmoticonFilterProxyModel::EmoticonFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(EmoticonModel::IdentifierRole);
    sort(0);
}

EmoticonFilterProxyModel::~EmoticonFilterProxyModel() = default;

bool EmoticonFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const QString leftString = sourceModel()->data(left, EmoticonModel::IdentifierRole).toString();
        const QString rightString = sourceModel()->data(right, EmoticonModel::IdentifierRole).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return false;
    }
}

#include "moc_emoticonfilterproxymodel.cpp"
