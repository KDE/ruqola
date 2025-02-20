/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbuttontest.h"
#include "actionbuttons/actionbutton.h"
#include <QTest>
QTEST_GUILESS_MAIN(ActionButtonTest)
ActionButtonTest::ActionButtonTest(QObject *parent)
    : QObject{parent}
{
}

void ActionButtonTest::shouldHaveDefaultValues()
{
    ActionButton b;
    QVERIFY(b.appId().isEmpty());
    QVERIFY(b.actionId().isEmpty());
    QVERIFY(b.labelI18n().isEmpty());
    QCOMPARE(b.roomTypeFilter(), ActionButton::RoomTypeFilter::Unknown);
}

#include "moc_actionbuttontest.cpp"
