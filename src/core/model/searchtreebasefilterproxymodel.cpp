/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchtreebasefilterproxymodel.h"
using namespace Qt::Literals::StringLiterals;

SearchTreeBaseFilterProxyModel::SearchTreeBaseFilterProxyModel(QAbstractListModel *model, QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setSourceModel(model);

    setFilterCaseSensitivity(Qt::CaseInsensitive);
    sort(0, Qt::DescendingOrder);
}

SearchTreeBaseFilterProxyModel::~SearchTreeBaseFilterProxyModel() = default;

void SearchTreeBaseFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}

void SearchTreeBaseFilterProxyModel::clearFilter()
{
    setFilterFixedString(QString());
}

bool SearchTreeBaseFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    if (mSortFunction) {
        bool useDefaultLessThan = false;
        const bool result = mSortFunction(source_left, source_right, useDefaultLessThan);
        if (!useDefaultLessThan) {
            return result;
        }
    }
    return QSortFilterProxyModel::lessThan(source_left, source_right);
}

void SearchTreeBaseFilterProxyModel::setSortFunction(SortFunction fn)
{
    mSortFunction = std::move(fn);
}

#include "moc_searchtreebasefilterproxymodel.cpp"
