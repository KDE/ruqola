/*
   Copyright (c) 2021 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#pragma once

#include "libruqolacore_export.h"
#include <QString>
#include <memory>

class QFile;
class QSqlTableModel;
class Message;

class LIBRUQOLACORE_EXPORT LocalMessageLogger
{
public:
    LocalMessageLogger();

    void addMessage(const QString &accountName, const QString &roomName, const Message &message);
    void deleteMessage(const QString &accountName, const QString &roomName, const QString &messageId);
    std::unique_ptr<QSqlTableModel> createMessageModel(const QString &accountName, const QString &roomName) const;
    bool saveToFile(QFile &file, const QString &accountName, const QString &roomName) const;

    // only public for the unittest
    Q_REQUIRED_RESULT QString dbFileName(const QString &accountName, const QString &roomName) const;

private:
    QString m_basePath;
};
