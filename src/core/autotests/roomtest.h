/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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

#ifndef ROOMTEST_H
#define ROOMTEST_H

#include <QObject>

class RoomTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomTest(QObject *parent = nullptr);
    ~RoomTest() = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldSerialized();
    void shouldEmitSignals();

    void shouldChangeInputMessage();

    //Rooms subscriptions only
    void shouldParseRoom_data();
    void shouldParseRoom();

    //Rooms subscriptions + update room
    void shouldParseRoomAndUpdate_data();
    void shouldParseRoomAndUpdate();

    //Rooms subscriptions + update room + update subscription
    void shouldParseRoomAndUpdateSubscription_data();
    void shouldParseRoomAndUpdateSubscription();
};

#endif // ROOMTEST_H
