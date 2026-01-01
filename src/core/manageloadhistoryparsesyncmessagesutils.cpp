/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "manageloadhistoryparsesyncmessagesutils.h"

#include "rocketchataccount.h"
using namespace Qt::Literals::StringLiterals;

ManageLoadHistoryParseSyncMessagesUtils::ManageLoadHistoryParseSyncMessagesUtils(RocketChatAccount *account)
    : mRocketChatAccount(account)
{
}

ManageLoadHistoryParseSyncMessagesUtils::~ManageLoadHistoryParseSyncMessagesUtils() = default;

QList<QByteArray> ManageLoadHistoryParseSyncMessagesUtils::deletedMessages() const
{
    return mDeletedMessages;
}

void ManageLoadHistoryParseSyncMessagesUtils::setDeletedMessages(const QList<QByteArray> &newDeletedMessages)
{
    mDeletedMessages = newDeletedMessages;
}

QList<Message> ManageLoadHistoryParseSyncMessagesUtils::updatesMessages() const
{
    return mUpdatesMessages;
}

void ManageLoadHistoryParseSyncMessagesUtils::setUpdatesMessages(const QList<Message> &newUpdatesMessages)
{
    mUpdatesMessages = newUpdatesMessages;
}

void ManageLoadHistoryParseSyncMessagesUtils::parse(const QJsonObject &obj)
{
    mDeletedMessages.clear();
    mUpdatesMessages.clear();
    const QJsonObject result = obj["result"_L1].toObject();
    const QJsonArray deleteArray = result["deleted"_L1].toArray();
    for (int i = 0, total = deleteArray.size(); i < total; ++i) {
        const QJsonObject o = deleteArray.at(i).toObject();
        mDeletedMessages.append(o["_id"_L1].toString().toLatin1());
    }

    QList<Message> updatedMessages;
    const QJsonArray updatedArray = result["updated"_L1].toArray();
    for (int i = 0, total = updatedArray.size(); i < total; ++i) {
        const QJsonObject o = updatedArray.at(i).toObject();
        Message m;
        m.parseMessage(o, true, mRocketChatAccount ? mRocketChatAccount->emojiManager() : nullptr);
        updatedMessages.append(std::move(m));
    }
    mUpdatesMessages = std::move(updatedMessages);
}
