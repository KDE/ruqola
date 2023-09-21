/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "globaldatabase.h"
#include "libruqolacore_export.h"
#include "messages/message.h"
#include <QString>
#include <QVector>
#include <memory>
class LocalMessageLogger;
class LocalMessageDatabase;
class LocalRoomsDatabase;
class LocalAccountDatabase;
class Message;
class Room;
class LIBRUQOLACORE_EXPORT LocalDatabaseManager
{
public:
    LocalDatabaseManager();
    ~LocalDatabaseManager();

    void deleteMessage(const QString &accountName, const QString &roomName, const QString &messageId);
    void addMessage(const QString &accountName, const QString &roomName, const Message &m);

    void addRoom(const QString &accountName, Room *room);
    void deleteRoom(const QString &accountName, const QString &roomId);

    void updateTimeStamp(const QString &accountName, const QString &roomName, qint64 timestamp, GlobalDatabase::TimeStampType type);
    void removeTimeStamp(const QString &accountName, const QString &roomName, GlobalDatabase::TimeStampType type);
    [[nodiscard]] qint64 timeStamp(const QString &accountName, const QString &roomName, GlobalDatabase::TimeStampType type);

    [[nodiscard]] QVector<Message>
    loadMessages(const QString &accountName, const QString &roomName, qint64 startId, qint64 endId, qint64 numberElements, EmojiManager *emojiManager) const;

    void updateAccount(const QString &accountName, const QByteArray &ba, qint64 timeStamp);
    void deleteAccount(const QString &accountName);

    [[nodiscard]] QByteArray jsonAccount(const QString &accountName);

private:
    std::unique_ptr<LocalMessageLogger> mMessageLogger;
    std::unique_ptr<LocalMessageDatabase> mMessagesDatabase;
    std::unique_ptr<LocalRoomsDatabase> mRoomsDatabase;
    std::unique_ptr<LocalAccountDatabase> mAccountDatabase;
    std::unique_ptr<GlobalDatabase> mGlobalDatabase;
};
