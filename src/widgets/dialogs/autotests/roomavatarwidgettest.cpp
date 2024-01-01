/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomavatarwidgettest.h"
#include "dialogs/roomavatarwidget.h"
#include <QTest>
QTEST_MAIN(RoomAvatarWidgetTest)
RoomAvatarWidgetTest::RoomAvatarWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void RoomAvatarWidgetTest::shouldHaveDefaultValues()
{
    RoomAvatarWidget w;
    QVERIFY(w.roomAvatar().isEmpty());
    QVERIFY(!w.wasChanged());
    QCOMPARE(w.size(), QSize(120, 120));
    QCOMPARE(w.iconSize(), QSize(100, 100));
}

#include "moc_roomavatarwidgettest.cpp"
