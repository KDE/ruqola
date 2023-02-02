/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QString>

class Message;
class LocalMessageDatabase
{
public:
    LocalMessageDatabase();
    ~LocalMessageDatabase();
    void deleteMessage(const QString &accountName, const QString &_roomName, const QString &messageId);
    void addMessage(const QString &accountName, const QString &_roomName, const Message &m);

    // only public for the unittest
    Q_REQUIRED_RESULT QString dbFileName(const QString &accountName, const QString &roomName) const;

private:
    const QString mBasePath;
};
