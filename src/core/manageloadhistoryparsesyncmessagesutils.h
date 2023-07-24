/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT QVector<Message> deletedMessages() const;
    void setDeletedMessages(const QVector<Message> &newDeletedMessages);

    Q_REQUIRED_RESULT QVector<Message> updatesMessages() const;
    void setUpdatesMessages(const QVector<Message> &newUpdatesMessages);

    void parse(const QJsonObject &obj);

private:
    QVector<Message> mDeletedMessages;
    QVector<Message> mUpdatesMessages;
    RocketChatAccount *const mRocketChatAccount;
};
