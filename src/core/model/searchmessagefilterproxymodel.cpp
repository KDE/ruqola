/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "searchmessagefilterproxymodel.h"
#include "searchmessagemodel.h"

SearchMessageFilterProxyModel::SearchMessageFilterProxyModel(SearchMessageModel *model, QObject *parent)
    : QSortFilterProxyModel(parent)
    , mSearchMessageModel(model)
{
    setSourceModel(mSearchMessageModel);
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(SearchMessageModel::Timestamp);
    sort(0, Qt::DescendingOrder);
    connect(mSearchMessageModel, &SearchMessageModel::stringNotFoundChanged, this, &SearchMessageFilterProxyModel::stringNotFoundChanged);
}

SearchMessageFilterProxyModel::~SearchMessageFilterProxyModel()
{
}

QHash<int, QByteArray> SearchMessageFilterProxyModel::roleNames() const
{
    if (QAbstractItemModel *source = sourceModel()) {
        return source->roleNames();
    }
    return QHash<int, QByteArray>();
}

bool SearchMessageFilterProxyModel::stringNotFound() const
{
    //FIXME return mSearchMessageModel->stringNotFound();
    return false;
}
