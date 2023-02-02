/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localmessagedatabase.h"
#include "localdatabaseutils.h"
#include "messages/message.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>

static const char s_schemaMessageDataBase[] = "CREATE TABLE LOGS (messageId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, json TEXT)";
enum class Fields {
    MessageId,
    TimeStamp,
    Json,
}; // in the same order as the table

LocalMessageDatabase::LocalMessageDatabase()
    : mBasePath(LocalDatabaseUtils::localMessageDatabasePath())
{
}

LocalMessageDatabase::~LocalMessageDatabase() = default;

QString LocalMessageDatabase::dbFileName(const QString &accountName, const QString &roomName) const
{
    const QString dirPath = mBasePath + accountName;
    return dirPath + QLatin1Char('/') + roomName + QStringLiteral(".sqlite");
}

void LocalMessageDatabase::addMessage(const QString &accountName, const QString &_roomName, const Message &m)
{
    // TODO store message.
}

void LocalMessageDatabase::deleteMessage(const QString &accountName, const QString &_roomName, const QString &messageId)
{
    // TODO delete message
}
