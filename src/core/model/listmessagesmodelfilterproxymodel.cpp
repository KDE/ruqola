/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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
#include <KLocalizedString>

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

QString ListMessagesModelFilterProxyModel::title() const
{
    switch(listMessageType()) {
    case ListMessagesModel::Unknown:
        return {};
    case ListMessagesModel::StarredMessages:
        return i18n("Starred Messages");
    case ListMessagesModel::SnipperedMessages:
        return i18n("Snippered Messages");
    case ListMessagesModel::PinnedMessages:
        return i18n("Pinned Messages");
    }
    return {};
}

ListMessagesModel::ListMessageType ListMessagesModelFilterProxyModel::listMessageType() const
{
    return mModel->listMessageType();
}

ListMessagesModel *ListMessagesModelFilterProxyModel::listMessageModel() const
{
    return mModel;
}

bool ListMessagesModelFilterProxyModel::hasFullList() const
{
    return mModel->hasFullList();
}
