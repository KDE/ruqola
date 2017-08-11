/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#ifndef MESSAGE_H
#define MESSAGE_H

#include "libruqola_private_export.h"
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_TESTS_EXPORT Message
{
public:
    Message();
    void parseMessage(const QJsonObject &o);

    // To be used in ID find: message ID
    inline bool operator==(const Message &other) const
    {
        return other.messageID == messageID;
    }

    // To be used in sorted insert: timestamp
    inline bool operator<(const Message &other) const
    {
        return timestamp < other.timestamp;
    }

    //Need for autotest as operator== is already defined
    bool isEqual(const Message &other) const;

    //Message Object Fields

    // _id
    QString messageID;

    // rid
    QString roomID;

    // msg
    QString message;

    // ts
    qint64 timestamp = -1;

    // u
    QString username;
    QString userID;

    // _updatedAt
    qint64 updatedAt = -1;

    // editedAt
    qint64 editedAt = -1;

    // editedBy
    QString editedByUsername;
    QString editedByUserID;

    // urls
    QString url;
    QString meta;
    QString headers;
    QString parsedUrl;

    // attachments
    QString imageUrl;
    QString color;

    // alias
    QString alias;

    // avatar
    QString avatar;
    QString systemMessageType;

    // groupable
    bool groupable = false;

    // parseUrls
    bool parseUrls = false;

    bool systemMessage = false;
};
#endif // MESSAGE_H
