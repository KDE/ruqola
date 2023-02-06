/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "threadmessagemodel.h"

#include "listmessages.h"

ThreadMessageModel::ThreadMessageModel(const QString &roomID, RocketChatAccount *account, Room *room, QObject *parent)
    : MessageModel(roomID, account, room, parent)
{
}

ThreadMessageModel::~ThreadMessageModel() = default;

QString ThreadMessageModel::threadMessageId() const
{
    return mThreadMessageId;
}

void ThreadMessageModel::setThreadMessageId(const QString &threadMessageId)
{
    mThreadMessageId = threadMessageId;
}

void ThreadMessageModel::parse(const QJsonObject &obj)
{
    ListMessages threadmessages;
    threadmessages.parseMessages(obj);
    mTotal = threadmessages.total();
    QVector<Message> listMessage = threadmessages.listMessages();
    if (!mPreviewMessage.messageId().isEmpty()) {
        listMessage.prepend(mPreviewMessage);
    }
    addMessages(listMessage);
}

void ThreadMessageModel::parseThreadMessages(const QJsonObject &obj)
{
    clear();
    parse(obj);
}

void ThreadMessageModel::loadMoreThreadMessages(const QJsonObject &obj)
{
    parse(obj);
}

int ThreadMessageModel::total() const
{
    return mTotal;
}

void ThreadMessageModel::setTotal(int total)
{
    mTotal = total;
}

void ThreadMessageModel::setPreviewMessage(const Message &message)
{
    mPreviewMessage = message;
}
