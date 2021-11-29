/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    QVERIFY(!w.readOnly());
}
