/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localmessagedatabase.h"
#include "messages/message.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>

LocalMessageDatabase::LocalMessageDatabase()
{
}

LocalMessageDatabase::~LocalMessageDatabase() = default;

void LocalMessageDatabase::addMessage(const QString &accountName, const QString &_roomName, const Message &m)
{
    // TODO store message.
}

void LocalMessageDatabase::deleteMessage(const QString &accountName, const QString &_roomName, const QString &messageId)
{
    // TODO delete message
}
