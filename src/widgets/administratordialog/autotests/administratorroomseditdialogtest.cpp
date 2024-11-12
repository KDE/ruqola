/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroomseditdialogtest.h"
#include "administratordialog/rooms/administratorroomseditdialog.h"
#include <QTest>
QTEST_MAIN(AdministratorRoomsEditDialogTest)
AdministratorRoomsEditDialogTest::AdministratorRoomsEditDialogTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorRoomsEditDialogTest::shouldHaveDefaultValues()
{
    AdministratorRoomsEditDialog d(AdministratorRoomsEditDialog::RoomType::Channel);
    QCOMPARE(d.roomType(), AdministratorRoomsEditDialog::RoomType::Channel);
    // TODO
}

#include "moc_administratorroomseditdialogtest.cpp"
