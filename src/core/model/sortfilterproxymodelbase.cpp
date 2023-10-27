/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sortfilterproxymodelbase.h"
#include "config-ruqola.h"
#if HAVE_TEXT_UTILS
#include <TextUtils/ConvertText>
#endif

SortFilterProxyModelBase::SortFilterProxyModelBase(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

SortFilterProxyModelBase::~SortFilterProxyModelBase() = default;

void SortFilterProxyModelBase::setFilterString(const QString &string)
{
#if HAVE_TEXT_UTILS
    mFilterString = TextUtils::ConvertText::normalize(string);
#else
    mFilterString = string;
#endif
    invalidate();
}

#include "moc_sortfilterproxymodelbase.cpp"
