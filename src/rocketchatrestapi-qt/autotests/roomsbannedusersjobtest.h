/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoomsBannedUsersJobTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomsBannedUsersJobTest(QObject *parent = nullptr);
    ~RoomsBannedUsersJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldNotStarting();
};
