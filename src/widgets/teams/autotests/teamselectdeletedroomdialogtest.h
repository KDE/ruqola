/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TeamSelectDeletedRoomDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit TeamSelectDeletedRoomDialogTest(QObject *parent = nullptr);
    ~TeamSelectDeletedRoomDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
