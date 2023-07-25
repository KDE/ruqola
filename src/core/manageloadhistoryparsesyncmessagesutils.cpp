/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

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

QVector<Message> ManageLoadHistoryParseSyncMessagesUtils::updatesMessages() const
{
    return mUpdatesMessages;
}

void ManageLoadHistoryParseSyncMessagesUtils::setUpdatesMessages(const QVector<Message> &newUpdatesMessages)
{
    mUpdatesMessages = newUpdatesMessages;
}

void ManageLoadHistoryParseSyncMessagesUtils::parse(const QJsonObject &obj)
{
    mDeletedMessages.clear();
    mUpdatesMessages.clear();
    const QJsonObject result = obj[QStringLiteral("result")].toObject();
    const QJsonArray deleteArray = result[QStringLiteral("deleted")].toArray();
    for (int i = 0, total = deleteArray.size(); i < total; ++i) {
        const QJsonObject o = deleteArray.at(i).toObject();
        mDeletedMessages.append(o[QStringLiteral("_id")].toString());
    }

    QVector<Message> updatedMessages;
    const QJsonArray updatedArray = result[QStringLiteral("updated")].toArray();
    for (int i = 0, total = updatedArray.size(); i < total; ++i) {
        const QJsonObject o = updatedArray.at(i).toObject();
        Message m(mRocketChatAccount ? mRocketChatAccount->emojiManager() : nullptr);
        m.parseMessage(o, true);
        updatedMessages.append(m);
    }
    mUpdatesMessages = std::move(updatedMessages);
}
