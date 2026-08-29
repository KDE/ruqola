/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
#include <QList>
#include <QString>
#include <memory>

class QFile;
class QSqlTableModel;
class Message;
class LIBRUQOLACORE_EXPORT LocalMessageLogger : public LocalDatabaseBase
{
public:
    LocalMessageLogger();

    void addMessage(const QString &accountName, const QByteArray &roomId, const Message &message);
    // Stores the whole list in a single transaction, reusing one prepared statement.
    void addMessages(const QString &accountName, const QByteArray &roomId, const QList<Message> &messages);
    void deleteMessage(const QString &accountName, const QByteArray &roomId, const QString &messageId);
    [[nodiscard]] std::unique_ptr<QSqlTableModel> createMessageModel(const QString &accountName, const QByteArray &roomId) const;
    [[nodiscard]] bool saveToFile(QFile &file, const QString &accountName, const QByteArray &roomId) const;

protected:
    [[nodiscard]] QString schemaDataBase() const override;

private:
    [[nodiscard]] static LIBRUQOLACORE_NO_EXPORT QString generateTextFromMessage(const Message &m);
};
