/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "listmessagesmodelfilterproxymodel.h"
#include "listmessagesmodel.h"

ListMessagesModelFilterProxyModel::ListMessagesModelFilterProxyModel(ListMessagesModel *model, QObject *parent)
    : QSortFilterProxyModel(parent)
    , mModel(model)
{
    setSourceModel(mModel);
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(ListMessagesModel::OriginalMessage);
    sort(0, Qt::DescendingOrder);
    connect(mModel, &ListMessagesModel::hasFullListChanged, this, &ListMessagesModelFilterProxyModel::hasFullListChanged);
    connect(mModel, &ListMessagesModel::listMessageTypeChanged, this, &ListMessagesModelFilterProxyModel::listMessageTypeChanged);
    connect(mModel, &ListMessagesModel::totalChanged, this, &ListMessagesModelFilterProxyModel::totalChanged);
}

ListMessagesModelFilterProxyModel::~ListMessagesModelFilterProxyModel()
{
}

int ListMessagesModelFilterProxyModel::total() const
{
    return mModel->total();
}

QHash<int, QByteArray> ListMessagesModelFilterProxyModel::roleNames() const
{
    if (QAbstractItemModel *source = sourceModel()) {
        return source->roleNames();
    }
    return QHash<int, QByteArray>();
}

void ListMessagesModelFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}

ListMessagesModel::ListMessageType ListMessagesModelFilterProxyModel::listMessageType() const
{
    return mModel->listMessageType();
}

bool ListMessagesModelFilterProxyModel::hasFullList() const
{
    return mModel->hasFullList();
}
