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

#ifndef ROCKETCHATMESSAGETEST_H
#define ROCKETCHATMESSAGETEST_H

#include <QObject>

class RocketChatMessageTest : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatMessageTest(QObject *parent = nullptr);
    ~RocketChatMessageTest() = default;
private Q_SLOTS:
    void shouldGenerateSetTemporaryStatus();
    void shouldHideRoom();
    void shouldLeaveRoom();
    void shouldOpenRoom();
    void shouldUnarchiveRoom();
    void shouldArchiveRoom();
    void shouldEraseRoom();
    void shouldGetRoomRoles();
    void shouldSetRoomTopic();
    void shouldToggleFavorite();
    void shouldSetRoomName();
    void shouldSetRoomDescription();
#if 0
    RocketChatMessage::RocketChatMessageResult setRoomIsReadOnly(const QString &roomId, bool readOnly, quint64 id);
    RocketChatMessage::RocketChatMessageResult setRoomHasSystemMessages(const QString &roomId, bool systemMessages, quint64 id);
    RocketChatMessage::RocketChatMessageResult setRoomIsDefault(const QString &roomId, bool isDefault, quint64 id);
    RocketChatMessage::RocketChatMessageResult setRoomJoinCode(const QString &roomId, const QString &joinCode, quint64 id);
    RocketChatMessage::RocketChatMessageResult readMessages(const QString &roomID, quint64 id);
    RocketChatMessage::RocketChatMessageResult joinRoom(const QString &roomId, const QString &accessCode, quint64 id);
    RocketChatMessage::RocketChatMessageResult informTypingStatus(const QString &roomId, const QString &userId, bool typingStatus, quint64 id);
    RocketChatMessage::RocketChatMessageResult getSubscriptions(const QDateTime &lastUpdate, quint64 id);
    RocketChatMessage::RocketChatMessageResult setDefaultStatus(RocketChatMessage::PresenceStatus status, quint64 id);
    RocketChatMessage::RocketChatMessageResult setTemporaryStatus(RocketChatMessage::PresenceStatus status, quint64 id);
#endif



private:
    void compareFile(const QString &data, const QString &name);
};

#endif // ROCKETCHATMESSAGETEST_H
