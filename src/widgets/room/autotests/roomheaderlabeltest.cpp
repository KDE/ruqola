/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
