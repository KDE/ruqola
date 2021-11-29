/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QObject>

class TeamSearchRoomDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit TeamSearchRoomDialogTest(QObject *parent = nullptr);
    ~TeamSearchRoomDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
