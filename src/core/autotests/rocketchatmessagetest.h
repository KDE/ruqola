/*
   Copyright (c) 2017-2021 Laurent Montel <montel@kde.org>

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

#include <QObject>

class RocketChatMessageTest : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatMessageTest(QObject *parent = nullptr);
    ~RocketChatMessageTest() override = default;
private Q_SLOTS:
    void shouldJoinRoom();
    void shouldSetDefaultStatus();
    void shouldInformTypingStatus();

    void shouldUserAutoComplete();
    void shouldSendFileMessage();

#if 0
    RocketChatMessage::RocketChatMessageResult RocketChatMessage::getUsersOfRoom(const QString &roomId, bool showAll, quint64 id)
    RocketChatMessage::RocketChatMessageResult setRoomHasSystemMessages(const QString &roomId, bool systemMessages, quint64 id);
    RocketChatMessage::RocketChatMessageResult readMessages(const QString &roomID, quint64 id);
    RocketChatMessage::RocketChatMessageResult getSubscriptions(const QDateTime &lastUpdate, quint64 id);
#endif

    void shouldSplotlightWithoutUsers();
    void shouldSplotlightWithoutRooms();

    void inputChannelAutocomplete();

    void inputUserAutocomplete();

    void blockUser();
    void unBlockUser();
    void deleteFileMessage();

private:
    void compareFile(const QString &data, const QString &name);
};

