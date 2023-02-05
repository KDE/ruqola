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
}

void LocalDatabaseManager::deleteMessage(const QString &accountName, const QString &roomName, const QString &messageId)
{
    mMessageLogger->deleteMessage(accountName, roomName, messageId);
    mMessagesDatabase->deleteMessage(accountName, roomName, messageId);
}
