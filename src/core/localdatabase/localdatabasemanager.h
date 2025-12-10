/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountroomsettings.h"
#include "globaldatabase.h"
#include "libruqolacore_export.h"
#include "messages/message.h"
#include <QList>
#include <QMap>
#include <QString>
#include <memory>
class LocalMessageLogger;
class LocalMessagesDatabase;
class LocalRoomsDatabase;
class LocalAccountsDatabase;
class E2EDataBase;
class Message;
class Room;
class LocalRoomPendingTypedInfoDatabase;
namespace RocketChatRestApi
{
class AbstractLogger;
}
class LIBRUQOLACORE_EXPORT LocalDatabaseManager
{
public:
    explicit LocalDatabaseManager(bool migrateDataBase = false);
    ~LocalDatabaseManager();

    void deleteMessage(const QString &accountName, const QByteArray &roomId, const QByteArray &messageId);
    void addMessage(const QString &accountName, const QByteArray &roomId, const Message &m);

    void addRoom(const QString &accountName, Room *room);
    void deleteRoom(const QString &accountName, const QByteArray &roomId);

    [[nodiscard]] qint64 timeStamp(const QString &accountName, const QByteArray &roomId, GlobalDatabase::TimeStampType type);

    [[nodiscard]] QList<Message>
    loadMessages(const QString &accountName, const QByteArray &roomId, qint64 startId, qint64 endId, qint64 numberElements, EmojiManager *emojiManager) const;

    void updateAccount(const QString &accountName, const QByteArray &ba, qint64 timeStamp);
    void deleteAccount(const QString &accountName);

    [[nodiscard]] QByteArray jsonAccount(const QString &accountName);

    void setDatabaseLogger(RocketChatRestApi::AbstractLogger *logger);

    [[nodiscard]] LocalMessagesDatabase *messagesDatabase() const;

    [[nodiscard]] LocalRoomsDatabase *roomsDatabase() const;

    [[nodiscard]] LocalAccountsDatabase *accountDatabase() const;

    [[nodiscard]] GlobalDatabase *globalDatabase() const;

    [[nodiscard]] E2EDataBase *e2EDatabase() const;

    [[nodiscard]] LocalRoomPendingTypedInfoDatabase *roomPendingTypedInfoDatabase() const;

    void updateRoomPendingTypedInfo(const QString &accountName, const QByteArray &roomId, const AccountRoomSettings::PendingTypedInfo &pendingTypedInfo);
    void deleteRoomPendingTypedInfo(const QString &accountName, const QByteArray &roomId);

    [[nodiscard]] QMap<QByteArray /*RoomId*/, AccountRoomSettings::PendingTypedInfo> loadRoomPendingTypedInfo(const QString &accountName) const;

    [[nodiscard]] QList<QByteArray> loadRooms(const QString &accountName);

private:
    LIBRUQOLACORE_NO_EXPORT void handleMigration();
    std::unique_ptr<LocalMessageLogger> mMessageLogger;
    std::unique_ptr<LocalMessagesDatabase> mMessagesDatabase;
    std::unique_ptr<LocalRoomsDatabase> mRoomsDatabase;
    std::unique_ptr<LocalAccountsDatabase> mAccountDatabase;
    std::unique_ptr<GlobalDatabase> mGlobalDatabase;
    std::unique_ptr<E2EDataBase> mE2EDatabase;
    std::unique_ptr<LocalRoomPendingTypedInfoDatabase> mRoomPendingTypedInfoDatabase;
};
