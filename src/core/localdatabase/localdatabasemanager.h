/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QString>
#include <memory>
class LocalMessageLogger;
class LocalMessageDatabase;
class Message;
class LIBRUQOLACORE_EXPORT LocalDatabaseManager
{
public:
    LocalDatabaseManager();
    ~LocalDatabaseManager();

    void deleteMessage(const QString &accountName, const QString &roomName, const QString &messageId);
    void addMessage(const QString &accountName, const QString &roomName, const Message &m);

private:
    std::unique_ptr<LocalMessageLogger> mMessageLogger;
    std::unique_ptr<LocalMessageDatabase> mMessageDatabase;
};
