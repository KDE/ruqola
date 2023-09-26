/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listmessagesmodel.h"

#include "listmessages.h"

ListMessagesModel::ListMessagesModel(const QString &roomID, RocketChatAccount *account, Room *room, QObject *parent)
    : MessagesModel(roomID, account, room, parent)
{
}

ListMessagesModel::~ListMessagesModel() = default;

void ListMessagesModel::parse(const QJsonObject &obj)
{
    ListMessages messages;
    QString parseMessageName;
    if (mListMessageType == MentionsMessages) {
        parseMessageName = QStringLiteral("messages");
    } else if (mListMessageType == ThreadsMessages || mListMessageType == UnreadThreadsMessages) {
        parseMessageName = QStringLiteral("threads");
    } else {
        parseMessageName = QStringLiteral("messages");
    }
    messages.parseMessages(obj, parseMessageName);
    mTotal = messages.total();
    addMessages(messages.listMessages(), true);
    setHasFullList(rowCount() == total());
}

void ListMessagesModel::parseListMessages(const QJsonObject &obj)
{
    clear();
    parse(obj);
}

void ListMessagesModel::loadMoreListMessages(const QJsonObject &obj)
{
    parse(obj);
}

int ListMessagesModel::total() const
{
    return mTotal;
}

void ListMessagesModel::setTotal(int total)
{
    if (mTotal != total) {
        mTotal = total;
        Q_EMIT totalChanged();
    }
}

bool ListMessagesModel::loadMoreListMessagesInProgress() const
{
    return mLoadingInProgress;
}

void ListMessagesModel::setLoadMoreListMessagesInProgress(bool inProgress)
{
    if (mLoadingInProgress != inProgress) {
        mLoadingInProgress = inProgress;
        Q_EMIT loadingInProgressChanged();
    }
}

void ListMessagesModel::setHasFullList(bool state)
{
    if (mHasFullList != state) {
        mHasFullList = state;
        Q_EMIT hasFullListChanged();
    }
}

bool ListMessagesModel::hasFullList() const
{
    return mHasFullList;
}

ListMessagesModel::ListMessageType ListMessagesModel::listMessageType() const
{
    return mListMessageType;
}

void ListMessagesModel::setListMessageType(ListMessageType listMessageType)
{
    if (mListMessageType != listMessageType) {
        mListMessageType = listMessageType;
        Q_EMIT listMessageTypeChanged();
    }
}

#include "moc_listmessagesmodel.cpp"
