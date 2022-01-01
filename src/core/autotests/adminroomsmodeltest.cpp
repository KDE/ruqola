/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminroomsmodeltest.h"
#include "model/adminroomsmodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(AdminRoomsModelTest)

AdminRoomsModelTest::AdminRoomsModelTest(QObject *parent)
    : QObject(parent)
{
}

void AdminRoomsModelTest::shouldHaveDefaultValues()
{
    AdminRoomsModel m;
    QVERIFY(m.adminRooms().isEmpty());
}

// TODO
