/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomcomboboxtest.h"
#include "room/usersinroomcombobox.h"
#include <QTest>
QTEST_MAIN(UsersInRoomComboBoxTest)
UsersInRoomComboBoxTest::UsersInRoomComboBoxTest(QObject *parent)
    : QObject(parent)
{
}

void UsersInRoomComboBoxTest::shouldHaveDefaultValues()
{
    UsersInRoomComboBox w;
    QVERIFY(w.count() > 0);
    QVERIFY(w.currentData().toString().isEmpty());
    // TODO
}
