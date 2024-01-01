/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class LocalRoomsDatabaseTest : public QObject
{
    Q_OBJECT
public:
    explicit LocalRoomsDatabaseTest(QObject *parent = nullptr);
    ~LocalRoomsDatabaseTest() override = default;

private Q_SLOTS:
    void initTestCase();
    void shouldDefaultValues();
    void shouldVerifyDbFileName();
    void shouldStoreRoomsSettings();
};
