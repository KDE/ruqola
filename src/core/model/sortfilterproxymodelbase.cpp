/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sortfilterproxymodelbase.h"

#include "config-ruqola.h"
#include <TextUtils/ConvertText>

SortFilterProxyModelBase::SortFilterProxyModelBase(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

SortFilterProxyModelBase::~SortFilterProxyModelBase() = default;

void SortFilterProxyModelBase::setFilterString(const QString &string)
{
    beginFilterChange();
    mFilterString = TextUtils::ConvertText::normalize(string);
    endFilterChange(Direction::Rows);
}

bool SortFilterProxyModelBase::contains(const QString &string) const
{
    const QString str = TextUtils::ConvertText::normalize(string);
    return str.contains(mFilterString, Qt::CaseInsensitive);
}

#include "moc_sortfilterproxymodelbase.cpp"
