/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include "messages/message.h"
#include <QJsonObject>
#include <QList>
class RocketChatAccount;

class LIBRUQOLACORE_TESTS_EXPORT ManageLoadHistoryParseSyncMessagesUtils
{
public:
    explicit ManageLoadHistoryParseSyncMessagesUtils(RocketChatAccount *account);
    ~ManageLoadHistoryParseSyncMessagesUtils();

    [[nodiscard]] QStringList deletedMessages() const;
    void setDeletedMessages(const QStringList &newDeletedMessages);

    [[nodiscard]] QList<Message> updatesMessages() const;
    void setUpdatesMessages(const QList<Message> &newUpdatesMessages);

    void parse(const QJsonObject &obj);

private:
    QStringList mDeletedMessages;
    QList<Message> mUpdatesMessages;
    RocketChatAccount *const mRocketChatAccount;
};
