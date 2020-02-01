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

#include "listmessagesmodel.h"

#include "listmessages.h"

ListMessagesModel::ListMessagesModel(const QString &roomID, RocketChatAccount *account, Room *room, QObject *parent)
    : MessageModel(roomID, account, room, parent)
{
}

ListMessagesModel::~ListMessagesModel()
{
}

QString ListMessagesModel::roomId() const
{
    return mRoomId;
}

void ListMessagesModel::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

void ListMessagesModel::parse(const QJsonObject &obj)
{
    ListMessages messages;
    QString parseMessageName;
    if (mListMessageType == MentionsMessages) {
        parseMessageName = QStringLiteral("mentions");
    } else {
        parseMessageName = QStringLiteral("messages");
    }
    messages.parseMessages(obj, parseMessageName);
    mTotal = messages.total();
    for (int i = 0, total = messages.count(); i < total; ++i) {
        addMessage(messages.at(i));
    }
    checkFullList();
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
    mLoadingInProgress = inProgress;
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

void ListMessagesModel::checkFullList()
{
    setHasFullList(rowCount() == total());
}

ListMessagesModel::ListMessageType ListMessagesModel::listMessageType() const
{
    return mListMessageType;
}

void ListMessagesModel::setListMessageType(const ListMessageType &listMessageType)
{
    if (mListMessageType != listMessageType) {
        mListMessageType = listMessageType;
        Q_EMIT listMessageTypeChanged();
    }
}
