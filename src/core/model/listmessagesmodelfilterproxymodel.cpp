/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listmessagesmodelfilterproxymodel.h"
#include <KLocalizedString>

ListMessagesModelFilterProxyModel::ListMessagesModelFilterProxyModel(ListMessagesModel *model, QObject *parent)
    : QSortFilterProxyModel(parent)
    , mModel(model)
{
    setSourceModel(mModel);

    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(ListMessagesModel::OriginalMessage);
    sort(0, Qt::DescendingOrder);
    connect(mModel, &ListMessagesModel::hasFullListChanged, this, &ListMessagesModelFilterProxyModel::hasFullListChanged);
    connect(mModel, &ListMessagesModel::listMessageTypeChanged, this, &ListMessagesModelFilterProxyModel::listMessageTypeChanged);
    connect(mModel, &ListMessagesModel::totalChanged, this, &ListMessagesModelFilterProxyModel::totalChanged);
    connect(mModel, &ListMessagesModel::loadingInProgressChanged, this, &ListMessagesModelFilterProxyModel::loadingInProgressChanged);
}

ListMessagesModelFilterProxyModel::~ListMessagesModelFilterProxyModel() = default;

int ListMessagesModelFilterProxyModel::total() const
{
    return mModel->total();
}

void ListMessagesModelFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}

QString ListMessagesModelFilterProxyModel::title() const
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

bool ListMessagesModelFilterProxyModel::loadMoreListMessagesInProgress() const
{
    return mModel->loadMoreListMessagesInProgress();
}

int ListMessagesModelFilterProxyModel::numberOfMessages() const
{
    return mModel->rowCount();
}

#include "moc_listmessagesmodelfilterproxymodel.cpp"
