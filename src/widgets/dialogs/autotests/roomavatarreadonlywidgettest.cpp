/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomavatarreadonlywidgettest.h"
#include "dialogs/roomavatarreadonlywidget.h"

#include <QTest>

QTEST_MAIN(RoomAvatarReadOnlyWidgetTest)
RoomAvatarReadOnlyWidgetTest::RoomAvatarReadOnlyWidgetTest(QObject *parent)
    : QObject{parent}
{

}

void RoomAvatarReadOnlyWidgetTest::shouldHaveDefaultValues()
{
    RoomAvatarReadOnlyWidget w;
    QCOMPARE(w.sizeHint(), QSize(120, 120));
}
