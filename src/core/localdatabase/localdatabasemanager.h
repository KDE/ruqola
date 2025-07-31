/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "globaldatabase.h"
#include "libruqolacore_export.h"
#include "messages/message.h"
#include <QList>
#include <QString>
#include <memory>
class LocalMessageLogger;
class LocalMessagesDatabase;
class LocalRoomsDatabase;
class LocalAccountsDatabase;
class Message;
class Room;
namespace RocketChatRestApi
{
class AbstractLogger;
}
class LIBRUQOLACORE_EXPORT LocalDatabaseManager
{
public:
    LocalDatabaseManager();
    ~LocalDatabaseManager();

    void deleteMessage(const QString &accountName, const QString &roomName, const QByteArray &messageId);
    void addMessage(const QString &accountName, const QString &roomName, const Message &m);

    void addRoom(const QString &accountName, Room *room);
    void deleteRoom(const QString &accountName, const QString &roomId);

    [[nodiscard]] qint64 timeStamp(const QString &accountName, const QString &roomName, GlobalDatabase::TimeStampType type);

    [[nodiscard]] QList<Message>
    loadMessages(const QString &accountName, const QString &roomName, qint64 startId, qint64 endId, qint64 numberElements, EmojiManager *emojiManager) const;

    void updateAccount(const QString &accountName, const QByteArray &ba, qint64 timeStamp);
    void deleteAccount(const QString &accountName);

    [[nodiscard]] QByteArray jsonAccount(const QString &accountName);

    void setDatabaseLogger(RocketChatRestApi::AbstractLogger *logger);

private:
    std::unique_ptr<LocalMessageLogger> mMessageLogger;
    std::unique_ptr<LocalMessagesDatabase> mMessagesDatabase;
    std::unique_ptr<LocalRoomsDatabase> mRoomsDatabase;
    std::unique_ptr<LocalAccountsDatabase> mAccountDatabase;
    std::unique_ptr<GlobalDatabase> mGlobalDatabase;
};
