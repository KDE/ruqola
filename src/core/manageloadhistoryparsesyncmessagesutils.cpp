/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "manageloadhistoryparsesyncmessagesutils.h"
#include "rocketchataccount.h"

ManageLoadHistoryParseSyncMessagesUtils::ManageLoadHistoryParseSyncMessagesUtils(RocketChatAccount *account)
    : mRocketChatAccount(account)
{
}

ManageLoadHistoryParseSyncMessagesUtils::~ManageLoadHistoryParseSyncMessagesUtils() = default;

QStringList ManageLoadHistoryParseSyncMessagesUtils::deletedMessages() const
{
    return mDeletedMessages;
}

void ManageLoadHistoryParseSyncMessagesUtils::setDeletedMessages(const QStringList &newDeletedMessages)
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
    const QJsonObject result = obj[QLatin1StringView("result")].toObject();
    const QJsonArray deleteArray = result[QLatin1StringView("deleted")].toArray();
    for (int i = 0, total = deleteArray.size(); i < total; ++i) {
        const QJsonObject o = deleteArray.at(i).toObject();
        mDeletedMessages.append(o[QLatin1StringView("_id")].toString());
    }

    QList<Message> updatedMessages;
    const QJsonArray updatedArray = result[QLatin1StringView("updated")].toArray();
    for (int i = 0, total = updatedArray.size(); i < total; ++i) {
        const QJsonObject o = updatedArray.at(i).toObject();
        Message m;
        m.parseMessage(o, true, mRocketChatAccount ? mRocketChatAccount->emojiManager() : nullptr);
        updatedMessages.append(m);
    }
    mUpdatesMessages = std::move(updatedMessages);
}
