/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabasemanager.h"
#include "localaccountsdatabase.h"
#include "localmessagelogger.h"
#include "localmessagesdatabase.h"
#include "localroomsdatabase.h"
#include "room.h"
#include "ruqolaglobalconfig.h"

LocalDatabaseManager::LocalDatabaseManager()
    : mMessageLogger(std::make_unique<LocalMessageLogger>())
    , mMessagesDatabase(std::make_unique<LocalMessagesDatabase>())
    , mRoomsDatabase(std::make_unique<LocalRoomsDatabase>())
    , mAccountDatabase(std::make_unique<LocalAccountsDatabase>())
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

void LocalDatabaseManager::deleteMessage(const QString &accountName, const QString &roomName, const QByteArray &messageId)
{
    const QString msgId = QString::fromLatin1(messageId);
    mMessageLogger->deleteMessage(accountName, roomName, msgId);
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mMessagesDatabase->deleteMessage(accountName, roomName, msgId);
        mGlobalDatabase->removeTimeStamp(accountName, roomName, GlobalDatabase::TimeStampType::MessageTimeStamp);
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

void LocalDatabaseManager::setDatabaseLogger(RocketChatRestApi::AbstractLogger *logger)
{
    mMessagesDatabase->setDatabaseLogger(logger);
    mRoomsDatabase->setDatabaseLogger(logger);
    mAccountDatabase->setDatabaseLogger(logger);
    mGlobalDatabase->setDatabaseLogger(logger);
}

void LocalDatabaseManager::addRoom(const QString &accountName, Room *room)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        mRoomsDatabase->updateRoom(accountName, room);
        // TODO verify it.
        mGlobalDatabase->insertOrReplaceTimeStamp(accountName,
                                                  QString::fromLatin1(room->roomId()),
                                                  room->lastMessageAt(),
                                                  GlobalDatabase::TimeStampType::RoomTimeStamp);
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

qint64 LocalDatabaseManager::timeStamp(const QString &accountName, const QString &roomName, GlobalDatabase::TimeStampType type)
{
    if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
        return mGlobalDatabase->timeStamp(accountName, roomName, type);
    }
    return -1;
}
