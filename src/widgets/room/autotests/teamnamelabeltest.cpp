/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamnamelabeltest.h"
#include "room/teamnamelabel.h"
#include <QTest>
QTEST_MAIN(TeamNameLabelTest)
TeamNameLabelTest::TeamNameLabelTest(QObject *parent)
    : QObject(parent)
{
}

void TeamNameLabelTest::shouldHaveDefaultValues()
{
    TeamNameLabel w;
    QCOMPARE(w.textFormat(), Qt::RichText);
    QCOMPARE(w.contextMenuPolicy(), Qt::NoContextMenu);
    QVERIFY(w.text().isEmpty());
    QCOMPARE(w.textInteractionFlags(), Qt::TextBrowserInteraction);
}
