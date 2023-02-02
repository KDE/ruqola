/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabasemanager.h"
#include "localmessagedatabase.h"
#include "localmessagelogger.h"

LocalDatabaseManager::LocalDatabaseManager()
    : mMessageLogger(std::make_unique<LocalMessageLogger>())
    , mMessageDatabase(std::make_unique<LocalMessageDatabase>())
{
}

LocalDatabaseManager::~LocalDatabaseManager() = default;

void LocalDatabaseManager::addMessage(const QString &accountName, const QString &roomName, const Message &m)
{
    mMessageLogger->addMessage(accountName, roomName, m);
    mMessageDatabase->addMessage(accountName, roomName, m);
}

void LocalDatabaseManager::deleteMessage(const QString &accountName, const QString &roomName, const QString &messageId)
{
    mMessageLogger->deleteMessage(accountName, roomName, messageId);
    mMessageDatabase->deleteMessage(accountName, roomName, messageId);
}
