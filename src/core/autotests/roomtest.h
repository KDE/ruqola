/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoomTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomTest(QObject *parent = nullptr);
    ~RoomTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldSerialized();
    void shouldEmitSignals();

    void shouldChangeInputMessage();

    // Rooms subscriptions only
    void shouldParseRoom_data();
    void shouldParseRoom();

    // Rooms subscriptions + update room
    void shouldParseRoomAndUpdate_data();
    void shouldParseRoomAndUpdate();

    // Rooms subscriptions + update room + update subscription
    void shouldParseRoomAndUpdateSubscription_data();
    void shouldParseRoomAndUpdateSubscription();
};
