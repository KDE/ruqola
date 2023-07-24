/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabasemanager.h"
#include "localaccountdatabase.h"
#include "localglobaldatabase.h"
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
    , mGlobalDatabase(std::make_unique<LocalGlobalDatabase>())
{
}

LocalDatabaseManager::~LocalDatabaseManager() = default;

void LocalDatabaseManager::addMessage(const QString &accountName, const QString &roomName, const Message &m)
{
    mMessageLogger->addMessage(accountName, roomName, m);
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mMessagesDatabase->addMessage(accountName, roomName, m);
        // Update timestamp.
        mGlobalDatabase->updateTimeStamp(accountName, roomName, m.timeStamp(), LocalGlobalDatabase::TimeStampType::MessageTimeStamp);
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

QVector<Message> LocalDatabaseManager::loadMessages(const QString &accountName,
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

void LocalDatabaseManager::addRoom(const QString &accountName, Room *room)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mRoomsDatabase->addRoom(accountName, room);
        // TODO mGlobalDatabase->updateTimeStamp(accountName, room->roomId(), room->timeStamp(), LocalGlobalDatabase::TimeStampType::RoomTimeStamp);
    }
}

void LocalDatabaseManager::deleteRoom(const QString &accountName, const QString &roomId)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mRoomsDatabase->deleteRoom(accountName, roomId);
        // Remove timestamp.
        mGlobalDatabase->removeTimeStamp(accountName, roomId, LocalGlobalDatabase::TimeStampType::RoomTimeStamp);
    }
}

void LocalDatabaseManager::updateTimeStamp(const QString &accountName, const QString &roomName, qint64 timestamp, LocalGlobalDatabase::TimeStampType type)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mGlobalDatabase->updateTimeStamp(accountName, roomName, timestamp, type);
    }
}

void LocalDatabaseManager::removeTimeStamp(const QString &accountName, const QString &roomName, LocalGlobalDatabase::TimeStampType type)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mGlobalDatabase->removeTimeStamp(accountName, roomName, type);
    }
}

qint64 LocalDatabaseManager::timeStamp(const QString &accountName, const QString &roomName, LocalGlobalDatabase::TimeStampType type)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        return mGlobalDatabase->timeStamp(accountName, roomName, type);
    }
    return -1;
}
