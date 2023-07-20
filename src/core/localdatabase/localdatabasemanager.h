/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "localglobaldatabase.h"
#include "messages/message.h"
#include <QString>
#include <QVector>
#include <memory>
class LocalMessageLogger;
class LocalMessageDatabase;
class LocalRoomsDatabase;
class LocalAccountDatabase;
class Message;
class Room;
class LIBRUQOLACORE_EXPORT LocalDatabaseManager
{
public:
    LocalDatabaseManager();
    ~LocalDatabaseManager();

    void deleteMessage(const QString &accountName, const QString &roomName, const QString &messageId);
    void addMessage(const QString &accountName, const QString &roomName, const Message &m);

    void addRoom(const QString &accountName, Room *room);
    void deleteRoom(const QString &accountName, const QString &roomId);

    void updateTimeStamp(const QString &accountName, const QString &roomName, qint64 timestamp, LocalGlobalDatabase::TimeStampType type);
    void removeTimeStamp(const QString &accountName, const QString &roomName, LocalGlobalDatabase::TimeStampType type);
    Q_REQUIRED_RESULT qint64 timeStamp(const QString &accountName, const QString &roomName, LocalGlobalDatabase::TimeStampType type);

    Q_REQUIRED_RESULT QVector<Message>
    loadMessages(const QString &accountName, const QString &roomName, qint64 startId, qint64 endId, qint64 numberElements) const;

private:
    std::unique_ptr<LocalMessageLogger> mMessageLogger;
    std::unique_ptr<LocalMessageDatabase> mMessagesDatabase;
    std::unique_ptr<LocalRoomsDatabase> mRoomsDatabase;
    std::unique_ptr<LocalAccountDatabase> mAccountDatabase;
    std::unique_ptr<LocalGlobalDatabase> mGlobalDatabase;
};
