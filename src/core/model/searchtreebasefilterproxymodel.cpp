/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchtreebasefilterproxymodel.h"
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

#include "moc_searchtreebasefilterproxymodel.cpp"
