/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
#include <QString>
#include <memory>

class QSqlTableModel;
class Message;
class LIBRUQOLACORE_EXPORT LocalMessageDatabase : public LocalDatabaseBase
{
public:
    LocalMessageDatabase();
    ~LocalMessageDatabase() override;
    void deleteMessage(const QString &accountName, const QString &_roomName, const QString &messageId);
    void addMessage(const QString &accountName, const QString &_roomName, const Message &m);

    Q_REQUIRED_RESULT std::unique_ptr<QSqlTableModel> createMessageModel(const QString &accountName, const QString &_roomName) const;

    Q_REQUIRED_RESULT QVector<Message>
    loadMessages(const QString &accountName, const QString &_roomName, qint64 startId = -1, qint64 endId = -1, qint64 numberElements = -1) const;

    Q_REQUIRED_RESULT static Message convertJsonToMessage(const QString &json);

protected:
    Q_REQUIRED_RESULT QString schemaDataBase() const override;
};
