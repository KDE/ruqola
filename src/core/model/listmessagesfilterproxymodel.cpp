/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

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

QString ListMessagesFilterProxyModel::title() const
{
    switch (listMessageType()) {
    case ListMessagesModel::Unknown:
        return {};
    case ListMessagesModel::StarredMessages:
        return i18n("Starred Messages");
    case ListMessagesModel::PinnedMessages:
        return i18n("Pinned Messages");
    case ListMessagesModel::MentionsMessages:
        return i18n("Mentions Messages");
    case ListMessagesModel::ThreadsMessages:
        return i18n("Threads Messages");
    case ListMessagesModel::UnreadThreadsMessages:
        return i18n("Unread Threads Messages");
    }
    return {};
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

#include "moc_listmessagesfilterproxymodel.cpp"
