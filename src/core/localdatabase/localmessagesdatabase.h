/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
#include <QString>
#include <memory>

class QSqlTableModel;
class Message;
class RocketChatAccount;
class EmojiManager;
class LIBRUQOLACORE_EXPORT LocalMessagesDatabase : public LocalDatabaseBase
{
public:
    LocalMessagesDatabase();
    ~LocalMessagesDatabase() override;
    void deleteMessage(const QString &accountName, const QByteArray &roomId, const QString &messageId);
    void addMessage(const QString &accountName, const QByteArray &roomId, const Message &m);

    [[nodiscard]] std::unique_ptr<QSqlTableModel> createMessageModel(const QString &accountName, const QByteArray &roomId) const;

    [[nodiscard]] QList<Message> loadMessages(const QString &accountName,
                                              const QByteArray &roomId,
                                              qint64 startId = -1,
                                              qint64 endId = -1,
                                              qint64 numberElements = -1,
                                              EmojiManager *emojiManager = nullptr) const;

    [[nodiscard]] static Message convertJsonToMessage(const QString &json, EmojiManager *emojiManager);

    [[nodiscard]] static QString generateQueryStr(qint64 startId, qint64 endId, qint64 numberElements);

    [[nodiscard]] QList<Message> loadMessages(RocketChatAccount *account, const QByteArray &roomId, qint64 startId, qint64 endId, qint64 numberElements) const;

protected:
    [[nodiscard]] QString schemaDataBase() const override;
};
