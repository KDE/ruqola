/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addusersinroomwidgettest.h"
#include "dialogs/addusersinroomwidget.h"
#include "misc/adduserswidget.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(AddUsersInRoomWidgetTest)

AddUsersInRoomWidgetTest::AddUsersInRoomWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AddUsersInRoomWidgetTest::shouldHaveDefaultValues()
{
    AddUsersInRoomWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAddUsersWidget = w.findChild<AddUsersWidget *>(QStringLiteral("mAddUsersWidget"));
    QVERIFY(mAddUsersWidget);

    QVERIFY(w.userIds().isEmpty());
    QVERIFY(w.userNames().isEmpty());
}

#include "moc_addusersinroomwidgettest.cpp"
