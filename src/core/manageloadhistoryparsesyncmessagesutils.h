/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include "messages/message.h"
#include <QJsonObject>
#include <QVector>
class RocketChatAccount;

class LIBRUQOLACORE_TESTS_EXPORT ManageLoadHistoryParseSyncMessagesUtils
{
public:
    explicit ManageLoadHistoryParseSyncMessagesUtils(RocketChatAccount *account);
    ~ManageLoadHistoryParseSyncMessagesUtils();

    [[nodiscard]] QStringList deletedMessages() const;
    void setDeletedMessages(const QStringList &newDeletedMessages);

    [[nodiscard]] QVector<Message> updatesMessages() const;
    void setUpdatesMessages(const QVector<Message> &newUpdatesMessages);

    void parse(const QJsonObject &obj);

private:
    QStringList mDeletedMessages;
    QVector<Message> mUpdatesMessages;
    RocketChatAccount *const mRocketChatAccount;
};
