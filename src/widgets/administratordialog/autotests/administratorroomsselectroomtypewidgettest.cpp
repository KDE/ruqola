/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroomsselectroomtypewidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto mDirectRooms = w.findChild<QCheckBox *>(u"mDirectRooms"_s);
    QVERIFY(mDirectRooms);
    QVERIFY(!mDirectRooms->text().isEmpty());
    QVERIFY(!mDirectRooms->isChecked());

    auto mPublicRooms = w.findChild<QCheckBox *>(u"mPublicRooms"_s);
    QVERIFY(mPublicRooms);
    QVERIFY(!mPublicRooms->text().isEmpty());
    QVERIFY(!mPublicRooms->isChecked());

    auto mPrivateRooms = w.findChild<QCheckBox *>(u"mPrivateRooms"_s);
    QVERIFY(mPrivateRooms);
    QVERIFY(!mPrivateRooms->text().isEmpty());
    QVERIFY(!mPrivateRooms->isChecked());

    auto mDiscussionRooms = w.findChild<QCheckBox *>(u"mDiscussionRooms"_s);
    QVERIFY(mDiscussionRooms);
    QVERIFY(!mDiscussionRooms->text().isEmpty());
    QVERIFY(!mDiscussionRooms->isChecked());

    auto mTeamRooms = w.findChild<QCheckBox *>(u"mTeamRooms"_s);
    QVERIFY(mTeamRooms);
    QVERIFY(!mTeamRooms->text().isEmpty());
    QVERIFY(!mTeamRooms->isChecked());
}

#include "moc_administratorroomsselectroomtypewidgettest.cpp"
