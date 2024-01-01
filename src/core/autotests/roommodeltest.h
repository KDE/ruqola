/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoomModelTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomModelTest(QObject *parent = nullptr);
    ~RoomModelTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldReturnRowCount();
    void shouldFindRoom();
    void shouldAddRoom();
    void shouldUpdateRoom();
    void shouldUpdateRoomFromQJsonObject();
    void shouldUpdateSubcriptionActionRemoved();
    void shouldUpdateSubcriptionActionInserted();
    void shouldUpdateSubcriptionActionUpdated();
    void shouldClear();
    void shouldReset();
    void shouldReturnDataDefault();
    void shouldReturnData();
    void shouldInsertRoom_data();
    void shouldInsertRoom();
};
