/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

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
    addMessages(threadmessages.listMessages());
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
