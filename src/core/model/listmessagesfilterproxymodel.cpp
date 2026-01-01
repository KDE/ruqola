/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

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

ListMessagesFilterProxyModel::FilteringByType ListMessagesFilterProxyModel::getFilteringByType() const
{
    return mFilteringByType;
}

void ListMessagesFilterProxyModel::setFilteringByType(FilteringByType newFilteringByType)
{
    if (mFilteringByType != newFilteringByType) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        beginFilterChange();
#endif
        mFilteringByType = newFilteringByType;
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
        invalidateFilter();
#endif
    }
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
    const QModelIndex sourceIndex = mModel->index(source_row, 0, source_parent);
    switch (mFilteringByType) {
    case FilteringByType::All:
        break;
    case FilteringByType::Unread: {
        const bool unread = sourceIndex.data(ListMessagesModel::Unread).toBool();
        if (!unread) {
            return false;
        }
        break;
    }
    case FilteringByType::Following: {
        const bool following = sourceIndex.data(ListMessagesModel::MessageReplies).toBool();
        if (!following) {
            return false;
        }
        break;
    }
    }
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

#include "moc_listmessagesfilterproxymodel.cpp"
