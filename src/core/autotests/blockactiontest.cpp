/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blockactiontest.h"
#include "messages/block/blockaction.h"
#include <QTest>
QTEST_GUILESS_MAIN(BlockActionTest)

BlockActionTest::BlockActionTest(QObject *parent)
    : QObject{parent}
{
}

void BlockActionTest::shouldHaveDefaultValues()
{
    BlockAction b;
    QVERIFY(b.actionId().isEmpty());
    QVERIFY(b.text().isEmpty());
    QVERIFY(b.type().isEmpty());
    QVERIFY(b.blockId().isEmpty());
    QVERIFY(b.value().isEmpty());
    QVERIFY(b.url().isEmpty());
    QVERIFY(!b.isValid());
}

#include "moc_blockactiontest.cpp"
