/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorybasefilterproxymodel.h"
DirectoryBaseFilterProxyModel::DirectoryBaseFilterProxyModel(QAbstractListModel *model, QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setSourceModel(model);

    setFilterCaseSensitivity(Qt::CaseInsensitive);
    sort(0, Qt::DescendingOrder);
}

DirectoryBaseFilterProxyModel::~DirectoryBaseFilterProxyModel() = default;

void DirectoryBaseFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}

void DirectoryBaseFilterProxyModel::clearFilter()
{
    setFilterFixedString(QString());
}
