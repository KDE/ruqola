/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomcomboboxtest.h"
#include "model/usersforroomfilterproxymodel.h"
#include "room/usersinroomcombobox.h"
#include <QTest>
QTEST_MAIN(UsersInRoomComboBoxTest)
UsersInRoomComboBoxTest::UsersInRoomComboBoxTest(QObject *parent)
    : QObject(parent)
{
}

void UsersInRoomComboBoxTest::shouldHaveDefaultValues()
{
    UsersInRoomComboBox w(false);
    QVERIFY(w.count() > 0);
    QCOMPARE(w.currentData().value<UsersForRoomFilterProxyModel::FilterUserType>(), UsersForRoomFilterProxyModel::FilterUserType::All);
}

#include "moc_usersinroomcomboboxtest.cpp"
