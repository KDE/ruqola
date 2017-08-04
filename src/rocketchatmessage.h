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

#ifndef ROCKETCHATMESSAGE_H
#define ROCKETCHATMESSAGE_H

#include <QString>
#include <QJsonDocument>

class RocketChatMessage
{
public:
    RocketChatMessage();
    ~RocketChatMessage() = default;

    struct RocketChatMessageResult
    {
        QString method;
        QString result;
        QJsonDocument jsonDocument;
    };

    void setJsonFormat(const QJsonDocument::JsonFormat &jsonFormat);

    RocketChatMessageResult hideRoom(const QString &roomID, quint64 id);
    RocketChatMessageResult leaveRoom(const QString &roomID, quint64 id);
    RocketChatMessageResult openRoom(const QString &roomID, quint64 id);
    RocketChatMessageResult unarchiveRoom(const QString &roomID, quint64 id);
    RocketChatMessageResult archiveRoom(const QString &roomID, quint64 id);
    RocketChatMessageResult eraseRoom(const QString &roomID, quint64 id);
    RocketChatMessageResult getRoomRoles(const QString &roomID, quint64 id);
    RocketChatMessageResult toggleFavorite(const QString &roomId, bool favorite, quint64 id);
    RocketChatMessageResult generateMethod(const QString &method, const QJsonDocument &params, quint64 id);
private:
    QJsonDocument::JsonFormat mJsonFormat;
};

#endif // ROCKETCHATMESSAGE_H
