/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "administratorroomsselectroomtypewidgettest.h"
#include "administratordialog/administratorroomsselectroomtypewidget.h"
#include <QHBoxLayout>
#include <QTest>
#include <QCheckBox>
QTEST_MAIN(AdministratorRoomsSelectRoomTypeWidgetTest)

AdministratorRoomsSelectRoomTypeWidgetTest::AdministratorRoomsSelectRoomTypeWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorRoomsSelectRoomTypeWidgetTest::shouldHaveDefaultValues()
{
    AdministratorRoomsSelectRoomTypeWidget w;

    auto *mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto *mDirectRooms = w.findChild<QCheckBox *>(QStringLiteral("mDirectRooms"));
    QVERIFY(mDirectRooms);
    QVERIFY(!mDirectRooms->text().isEmpty());
    QVERIFY(!mDirectRooms->isChecked());

    auto *mPublicRooms = w.findChild<QCheckBox *>(QStringLiteral("mPublicRooms"));
    QVERIFY(mPublicRooms);
    QVERIFY(!mPublicRooms->text().isEmpty());
    QVERIFY(!mPublicRooms->isChecked());

    auto *mPrivateRooms = w.findChild<QCheckBox *>(QStringLiteral("mPrivateRooms"));
    QVERIFY(mPrivateRooms);
    QVERIFY(!mPrivateRooms->text().isEmpty());
    QVERIFY(!mPrivateRooms->isChecked());

    auto *mDiscussionRooms = w.findChild<QCheckBox *>(QStringLiteral("mDiscussionRooms"));
    QVERIFY(mDiscussionRooms);
    QVERIFY(!mDiscussionRooms->text().isEmpty());
    QVERIFY(!mDiscussionRooms->isChecked());
}
