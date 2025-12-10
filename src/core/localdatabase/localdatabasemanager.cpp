/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabasemanager.h"
#include "e2edatabase.h"
#include "localaccountsdatabase.h"
#include "localdatabaseutils.h"
#include "localmessagelogger.h"
#include "localmessagesdatabase.h"
#include "localroompendingtypedinfodatabase.h"
#include "localroomsdatabase.h"
#include "room.h"
#include "ruqola_database_debug.h"
#include "ruqolaglobalconfig.h"
#include <QDir>

LocalDatabaseManager::LocalDatabaseManager(bool migrateDataBase)
    : mMessageLogger(std::make_unique<LocalMessageLogger>())
    , mMessagesDatabase(std::make_unique<LocalMessagesDatabase>())
    , mRoomsDatabase(std::make_unique<LocalRoomsDatabase>())
    , mAccountDatabase(std::make_unique<LocalAccountsDatabase>())
    , mGlobalDatabase(std::make_unique<GlobalDatabase>())
    , mE2EDatabase(std::make_unique<E2EDataBase>())
    , mRoomPendingTypedInfoDatabase(std::make_unique<LocalRoomPendingTypedInfoDatabase>())
{
    if (migrateDataBase) {
        handleMigration();
    }
}

LocalDatabaseManager::~LocalDatabaseManager() = default;

void LocalDatabaseManager::handleMigration()
{
    const QStringList lst = {LocalDatabaseUtils::localMessageLoggerPath(), LocalDatabaseUtils::localDatabasePath()};
    for (const QString &path : lst) {
        QDir dir(path);
        if (dir.exists()) {
            if (!dir.removeRecursively()) {
                qCWarning(RUQOLA_DATABASE_LOG) << "Impossible to remove database from " << dir.absolutePath();
            }
        }
    }
}

E2EDataBase *LocalDatabaseManager::e2EDatabase() const
{
    return mE2EDatabase.get();
}

LocalRoomPendingTypedInfoDatabase *LocalDatabaseManager::roomPendingTypedInfoDatabase() const
{
    return mRoomPendingTypedInfoDatabase.get();
}

void LocalDatabaseManager::addMessage(const QString &accountName, const QByteArray &roomId, const Message &m)
{
    mMessageLogger->addMessage(accountName, roomId, m);
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mMessagesDatabase->addMessage(accountName, roomId, m);
        // Update timestamp.
        mGlobalDatabase->insertOrReplaceTimeStamp(accountName, roomId, m.timeStamp(), GlobalDatabase::TimeStampType::MessageTimeStamp);
    }
}

void LocalDatabaseManager::deleteMessage(const QString &accountName, const QByteArray &roomId, const QByteArray &messageId)
{
    const QString msgId = QString::fromLatin1(messageId);
    mMessageLogger->deleteMessage(accountName, roomId, msgId);
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mMessagesDatabase->deleteMessage(accountName, roomId, msgId);
        mGlobalDatabase->removeTimeStamp(accountName, roomId, GlobalDatabase::TimeStampType::MessageTimeStamp);
    }
}

QList<Message> LocalDatabaseManager::loadMessages(const QString &accountName,
                                                  const QByteArray &roomId,
                                                  qint64 startId,
                                                  qint64 endId,
                                                  qint64 numberElements,
                                                  EmojiManager *emojiManager) const
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        return mMessagesDatabase->loadMessages(accountName, roomId, startId, endId, numberElements, emojiManager);
    }
    return {};
}

void LocalDatabaseManager::updateAccount(const QString &accountName, const QByteArray &ba, qint64 timeStamp)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mAccountDatabase->updateAccount(accountName, ba);
        if (timeStamp > -1) {
            mGlobalDatabase->insertOrReplaceTimeStamp(accountName, {}, timeStamp, GlobalDatabase::TimeStampType::AccountTimeStamp);
        }
    }
}

void LocalDatabaseManager::deleteAccount(const QString &accountName)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mAccountDatabase->deleteAccount(accountName);
        mGlobalDatabase->removeTimeStamp(accountName, QByteArray(), GlobalDatabase::TimeStampType::AccountTimeStamp);
    }
}

QByteArray LocalDatabaseManager::jsonAccount(const QString &accountName)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        return mAccountDatabase->jsonAccount(accountName);
    }
    return {};
}

void LocalDatabaseManager::setDatabaseLogger(RocketChatRestApi::AbstractLogger *logger)
{
    mMessagesDatabase->setDatabaseLogger(logger);
    mRoomsDatabase->setDatabaseLogger(logger);
    mAccountDatabase->setDatabaseLogger(logger);
    mGlobalDatabase->setDatabaseLogger(logger);
    mE2EDatabase->setDatabaseLogger(logger);
    mRoomPendingTypedInfoDatabase->setDatabaseLogger(logger);
}

LocalMessagesDatabase *LocalDatabaseManager::messagesDatabase() const
{
    return mMessagesDatabase.get();
}

LocalRoomsDatabase *LocalDatabaseManager::roomsDatabase() const
{
    return mRoomsDatabase.get();
}

LocalAccountsDatabase *LocalDatabaseManager::accountDatabase() const
{
    return mAccountDatabase.get();
}

GlobalDatabase *LocalDatabaseManager::globalDatabase() const
{
    return mGlobalDatabase.get();
}

void LocalDatabaseManager::addRoom(const QString &accountName, Room *room)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mRoomsDatabase->updateRoom(accountName, room);
        // TODO verify it.
        mGlobalDatabase->insertOrReplaceTimeStamp(accountName, room->roomId(), room->lastMessageAt(), GlobalDatabase::TimeStampType::RoomTimeStamp);
    }
}

void LocalDatabaseManager::deleteRoom(const QString &accountName, const QByteArray &roomId)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mRoomsDatabase->deleteRoom(accountName, roomId);
        // Remove timestamp.
        mGlobalDatabase->removeTimeStamp(accountName, roomId, GlobalDatabase::TimeStampType::RoomTimeStamp);
    }
}

qint64 LocalDatabaseManager::timeStamp(const QString &accountName, const QByteArray &roomId, GlobalDatabase::TimeStampType type)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        return mGlobalDatabase->timeStamp(accountName, roomId, type);
    }
    return -1;
}

void LocalDatabaseManager::updateRoomPendingTypedInfo(const QString &accountName,
                                                      const QByteArray &roomId,
                                                      const AccountRoomSettings::PendingTypedInfo &pendingTypedInfo)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mRoomPendingTypedInfoDatabase->updateRoomPendingTypedInfo(accountName, roomId, pendingTypedInfo);
    }
}

void LocalDatabaseManager::deleteRoomPendingTypedInfo(const QString &accountName, const QByteArray &roomId)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mRoomPendingTypedInfoDatabase->deleteRoomPendingTypedInfo(accountName, roomId);
    }
}

QMap<QByteArray /*RoomId*/, AccountRoomSettings::PendingTypedInfo> LocalDatabaseManager::loadRoomPendingTypedInfo(const QString &accountName) const
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        return mRoomPendingTypedInfoDatabase->loadRoomPendingTypedInfo(accountName);
    } else {
        return {};
    }
}

QList<QByteArray> LocalDatabaseManager::loadRooms(const QString &accountName)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        return mRoomsDatabase->loadRooms(accountName);
    } else {
        return {};
    }
}
