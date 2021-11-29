/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "roomheaderlabeltest.h"
#include "room/roomheaderlabel.h"
#include <QTest>
QTEST_MAIN(RoomHeaderLabelTest)
RoomHeaderLabelTest::RoomHeaderLabelTest(QObject *parent)
    : QObject(parent)
{
}

void RoomHeaderLabelTest::shouldHaveDefaultValues()
{
    RoomHeaderLabel w;
    QVERIFY(w.fullText().isEmpty());
    // TODO
}
