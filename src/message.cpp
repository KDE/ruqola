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

#include "message.h"

Message::Message()
{

}

void Message::parseMessage(const QJsonObject &o)
{
    const QString roomId = o.value(QStringLiteral("rid")).toString();

    //t ? I can't find it.
    const QString type = o.value(QStringLiteral("t")).toString();

    messageID = o.value(QStringLiteral("_id")).toString();
    roomID = roomId;
    message = o.value(QStringLiteral("msg")).toString();
    timestamp = (qint64)o.value(QStringLiteral("ts")).toObject().value(QStringLiteral("$date")).toDouble();
    username = o.value(QStringLiteral("u")).toObject().value(QStringLiteral("username")).toString();
    userID = o.value(QStringLiteral("u")).toObject().value(QStringLiteral("_id")).toString();
    updatedAt = o.value(QStringLiteral("_updatedAt")).toObject().value(QStringLiteral("$date")).toDouble();
    editedAt = o.value(QStringLiteral("editedAt")).toObject().value(QStringLiteral("$date")).toDouble();
    editedByUsername = o.value(QStringLiteral("editedBy")).toObject().value(QStringLiteral("username")).toString();
    editedByUserID = o.value(QStringLiteral("editedBy")).toObject().value(QStringLiteral("userID")).toString();
    url = o.value(QStringLiteral("urls")).toObject().value(QStringLiteral("url")).toString();
    meta = o.value(QStringLiteral("urls")).toObject().value(QStringLiteral("meta")).toString();
    headers = o.value(QStringLiteral("urls")).toObject().value(QStringLiteral("headers")).toString();
    parsedUrl = o.value(QStringLiteral("urls")).toObject().value(QStringLiteral("parsedUrl")).toString();
    imageUrl = o.value(QStringLiteral("attachments")).toObject().value(QStringLiteral("image_url")).toString();
    color = o.value(QStringLiteral("attachments")).toObject().value(QStringLiteral("color")).toString();
    alias = o.value(QStringLiteral("alias")).toString();
    avatar = o.value(QStringLiteral("avatar")).toString();
    groupable = o.value(QStringLiteral("groupable")).toBool();
    parseUrls = o.value(QStringLiteral("parseUrls")).toBool();

    if (type.isEmpty()) {
        systemMessage = false;
    } else {
        systemMessage = true;
        systemMessageType = type;
    }


}
