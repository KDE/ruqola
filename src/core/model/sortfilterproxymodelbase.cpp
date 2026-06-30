/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sortfilterproxymodelbase.h"

#include "config-ruqola.h"
#if HAVE_TEXT_UTILS
#include <TextUtils/ConvertText>
#endif

using namespace Qt::Literals::StringLiterals;
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

bool SortFilterProxyModelBase::contains(const QString &string) const
{
#if HAVE_TEXT_UTILS
    const QString str = TextUtils::ConvertText::normalize(string);
    return str.contains(mFilterString, Qt::CaseInsensitive);
#else
    const QString str = string;
    return str.contains(mFilterString, Qt::CaseInsensitive);
#endif
}

#include "moc_sortfilterproxymodelbase.cpp"
