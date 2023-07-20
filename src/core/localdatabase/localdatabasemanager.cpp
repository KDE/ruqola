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
    mMessagesDatabase->addMessage(accountName, roomName, m);
    // Update timestamp.
    // mGlobalDatabase->updateTimeStamp(accountName, roomName, m.timeStamp()); ???
}

void LocalDatabaseManager::deleteMessage(const QString &accountName, const QString &roomName, const QString &messageId)
{
    mMessageLogger->deleteMessage(accountName, roomName, messageId);
    mMessagesDatabase->deleteMessage(accountName, roomName, messageId);
}

QVector<Message>
LocalDatabaseManager::loadMessages(const QString &accountName, const QString &roomName, qint64 startId, qint64 endId, qint64 numberElements) const
{
    return mMessagesDatabase->loadMessages(accountName, roomName, startId, endId, numberElements);
}

void LocalDatabaseManager::addRoom(const QString &accountName, Room *room)
{
    mRoomsDatabase->addRoom(accountName, room);
}

void LocalDatabaseManager::deleteRoom(const QString &accountName, const QString &roomId)
{
    mRoomsDatabase->deleteRoom(accountName, roomId);
}

void LocalDatabaseManager::updateTimeStamp(const QString &accountName, const QString &roomName, qint64 timestamp, LocalGlobalDatabase::TimeStampType type)
{
    mGlobalDatabase->updateTimeStamp(accountName, roomName, timestamp, type);
}

void LocalDatabaseManager::removeTimeStamp(const QString &accountName, const QString &roomName, LocalGlobalDatabase::TimeStampType type)
{
    mGlobalDatabase->removeTimeStamp(accountName, roomName, type);
}

qint64 LocalDatabaseManager::timeStamp(const QString &accountName, const QString &roomName, LocalGlobalDatabase::TimeStampType type)
{
    return mGlobalDatabase->timeStamp(accountName, roomName, type);
}
