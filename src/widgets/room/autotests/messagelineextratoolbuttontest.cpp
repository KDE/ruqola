/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagelineextratoolbuttontest.h"
#include "room/messagelineextratoolbutton.h"
#include <QTest>
QTEST_MAIN(MessageLineExtraToolButtonTest)

MessageLineExtraToolButtonTest::MessageLineExtraToolButtonTest(QObject *parent)
    : QObject{parent}
{
}

void MessageLineExtraToolButtonTest::shouldHaveDefaultValues()
{
    MessageLineExtraToolButton b;
    QVERIFY(!b.toolTip().isEmpty());
    QVERIFY(b.autoRaise());
    QCOMPARE(b.popupMode(), QToolButton::InstantPopup);
}

#include "moc_messagelineextratoolbuttontest.cpp"
