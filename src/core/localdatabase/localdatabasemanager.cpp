/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabasemanager.h"
#include "localaccountdatabase.h"
#include "localmessagedatabase.h"
#include "localmessagelogger.h"
#include "localroomsdatabase.h"
#include "room.h"
#include "ruqolaglobalconfig.h"

LocalDatabaseManager::LocalDatabaseManager()
    : mMessageLogger(std::make_unique<LocalMessageLogger>())
    , mMessagesDatabase(std::make_unique<LocalMessageDatabase>())
    , mRoomsDatabase(std::make_unique<LocalRoomsDatabase>())
    , mAccountDatabase(std::make_unique<LocalAccountDatabase>())
    , mGlobalDatabase(std::make_unique<GlobalDatabase>())
{
}

LocalDatabaseManager::~LocalDatabaseManager() = default;

void LocalDatabaseManager::addMessage(const QString &accountName, const QString &roomName, const Message &m)
{
    mMessageLogger->addMessage(accountName, roomName, m);
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mMessagesDatabase->addMessage(accountName, roomName, m);
        // Update timestamp.
        mGlobalDatabase->insertOrReplaceTimeStamp(accountName, roomName, m.timeStamp(), GlobalDatabase::TimeStampType::MessageTimeStamp);
    }
}

void LocalDatabaseManager::deleteMessage(const QString &accountName, const QString &roomName, const QString &messageId)
{
    mMessageLogger->deleteMessage(accountName, roomName, messageId);
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mMessagesDatabase->deleteMessage(accountName, roomName, messageId);
        // TODO update timestamp ?
    }
}

QList<Message> LocalDatabaseManager::loadMessages(const QString &accountName,
                                                  const QString &roomName,
                                                  qint64 startId,
                                                  qint64 endId,
                                                  qint64 numberElements,
                                                  EmojiManager *emojiManager) const
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        return mMessagesDatabase->loadMessages(accountName, roomName, startId, endId, numberElements, emojiManager);
    }
    return {};
}

void LocalDatabaseManager::updateAccount(const QString &accountName, const QByteArray &ba, qint64 timeStamp)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mAccountDatabase->updateAccount(accountName, ba);
        if (timeStamp > -1) {
            mGlobalDatabase->insertOrReplaceTimeStamp(accountName, QString(), timeStamp, GlobalDatabase::TimeStampType::AccountTimeStamp);
        }
    }
}

void LocalDatabaseManager::deleteAccount(const QString &accountName)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mAccountDatabase->deleteAccount(accountName);
        mGlobalDatabase->removeTimeStamp(accountName, QString(), GlobalDatabase::TimeStampType::AccountTimeStamp);
    }
}

QByteArray LocalDatabaseManager::jsonAccount(const QString &accountName)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        return mAccountDatabase->jsonAccount(accountName);
    }
    return {};
}

void LocalDatabaseManager::addRoom(const QString &accountName, Room *room)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mRoomsDatabase->addRoom(accountName, room);
        // TODO verify it.
        mGlobalDatabase->insertOrReplaceTimeStamp(accountName, room->roomId(), room->lastMessageAt(), GlobalDatabase::TimeStampType::RoomTimeStamp);
    }
}

void LocalDatabaseManager::deleteRoom(const QString &accountName, const QString &roomId)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mRoomsDatabase->deleteRoom(accountName, roomId);
        // Remove timestamp.
        mGlobalDatabase->removeTimeStamp(accountName, roomId, GlobalDatabase::TimeStampType::RoomTimeStamp);
    }
}

void LocalDatabaseManager::updateTimeStamp(const QString &accountName, const QString &roomName, qint64 timestamp, GlobalDatabase::TimeStampType type)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mGlobalDatabase->insertOrReplaceTimeStamp(accountName, roomName, timestamp, type);
    }
}

void LocalDatabaseManager::removeTimeStamp(const QString &accountName, const QString &roomName, GlobalDatabase::TimeStampType type)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mGlobalDatabase->removeTimeStamp(accountName, roomName, type);
    }
}

qint64 LocalDatabaseManager::timeStamp(const QString &accountName, const QString &roomName, GlobalDatabase::TimeStampType type)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        return mGlobalDatabase->timeStamp(accountName, roomName, type);
    }
    return -1;
}
