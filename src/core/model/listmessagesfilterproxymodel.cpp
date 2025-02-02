/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listmessagesfilterproxymodel.h"
#include <KLocalizedString>

ListMessagesFilterProxyModel::ListMessagesFilterProxyModel(ListMessagesModel *model, QObject *parent)
    : QSortFilterProxyModel(parent)
    , mModel(model)
{
    setSourceModel(mModel);

    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(ListMessagesModel::OriginalMessage);
    sort(0, Qt::DescendingOrder);
    connect(mModel, &ListMessagesModel::hasFullListChanged, this, &ListMessagesFilterProxyModel::hasFullListChanged);
    connect(mModel, &ListMessagesModel::listMessageTypeChanged, this, &ListMessagesFilterProxyModel::listMessageTypeChanged);
    connect(mModel, &ListMessagesModel::totalChanged, this, &ListMessagesFilterProxyModel::totalChanged);
    connect(mModel, &ListMessagesModel::loadingInProgressChanged, this, &ListMessagesFilterProxyModel::loadingInProgressChanged);
}

ListMessagesFilterProxyModel::~ListMessagesFilterProxyModel() = default;

int ListMessagesFilterProxyModel::total() const
{
    return mModel->total();
}

void ListMessagesFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}

ListMessagesModel::ListMessageType ListMessagesFilterProxyModel::listMessageType() const
{
    return mModel->listMessageType();
}

ListMessagesModel *ListMessagesFilterProxyModel::listMessageModel() const
{
    return mModel;
}

bool ListMessagesFilterProxyModel::hasFullList() const
{
    return mModel->hasFullList();
}

bool ListMessagesFilterProxyModel::loadMoreListMessagesInProgress() const
{
    return mModel->loadMoreListMessagesInProgress();
}

int ListMessagesFilterProxyModel::numberOfMessages() const
{
    return mModel->rowCount();
}

void ListMessagesFilterProxyModel::clear()
{
    mModel->clear();
}

bool ListMessagesFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
#if 0
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    const QString typegroup = sourceIndex.data(ListMessagesModel::TypeGroup).toString();
    const QString fileName = sourceIndex.data(FilesForRoomModel::FileName).toString();
    const QString username = sourceIndex.data(FilesForRoomModel::UserName).toString();

    if (mSearchText.isEmpty() && mTypeGroup.isEmpty()) {
        return true;
    } else {
        const bool indexContains = fileName.contains(mSearchText) || username.contains(mSearchText);
        if (!mSearchText.isEmpty() && mTypeGroup.isEmpty()) {
            return indexContains;
        }
        if (!mTypeGroup.isEmpty()) {
            return (mTypeGroup == typegroup) && (indexContains);
        }
    }
    return false;
#endif
}

#include "moc_listmessagesfilterproxymodel.cpp"
