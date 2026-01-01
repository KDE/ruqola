/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomflowwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "common/flowlayout.h"
#include "room/usersinroomflowwidget.h"
#include <QTest>
QTEST_MAIN(UsersInRoomFlowWidgetTest)

UsersInRoomFlowWidgetTest::UsersInRoomFlowWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void UsersInRoomFlowWidgetTest::shouldHaveDefaultValues()
{
    UsersInRoomFlowWidget w;
    auto mFlowLayout = w.findChild<FlowLayout *>(u"mFlowLayout"_s);
    QVERIFY(mFlowLayout);
    QCOMPARE(mFlowLayout->spacing(), 0);
    QCOMPARE(mFlowLayout->contentsMargins(), QMargins{});
}

#include "moc_usersinroomflowwidgettest.cpp"
