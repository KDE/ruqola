/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroomsselectroomtypewidgettest.h"
#include "administratordialog/rooms/administratorroomsselectroomtypewidget.h"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QTest>
QTEST_MAIN(AdministratorRoomsSelectRoomTypeWidgetTest)

AdministratorRoomsSelectRoomTypeWidgetTest::AdministratorRoomsSelectRoomTypeWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorRoomsSelectRoomTypeWidgetTest::shouldHaveDefaultValues()
{
    AdministratorRoomsSelectRoomTypeWidget w;

    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto mDirectRooms = w.findChild<QCheckBox *>(QStringLiteral("mDirectRooms"));
    QVERIFY(mDirectRooms);
    QVERIFY(!mDirectRooms->text().isEmpty());
    QVERIFY(!mDirectRooms->isChecked());

    auto mPublicRooms = w.findChild<QCheckBox *>(QStringLiteral("mPublicRooms"));
    QVERIFY(mPublicRooms);
    QVERIFY(!mPublicRooms->text().isEmpty());
    QVERIFY(!mPublicRooms->isChecked());

    auto mPrivateRooms = w.findChild<QCheckBox *>(QStringLiteral("mPrivateRooms"));
    QVERIFY(mPrivateRooms);
    QVERIFY(!mPrivateRooms->text().isEmpty());
    QVERIFY(!mPrivateRooms->isChecked());

    auto mDiscussionRooms = w.findChild<QCheckBox *>(QStringLiteral("mDiscussionRooms"));
    QVERIFY(mDiscussionRooms);
    QVERIFY(!mDiscussionRooms->text().isEmpty());
    QVERIFY(!mDiscussionRooms->isChecked());

    auto mTeamRooms = w.findChild<QCheckBox *>(QStringLiteral("mTeamRooms"));
    QVERIFY(mTeamRooms);
    QVERIFY(!mTeamRooms->text().isEmpty());
    QVERIFY(!mTeamRooms->isChecked());
}
