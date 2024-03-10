/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blockactiontest.h"
#include "messages/blockaction.h"
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
}

#include "moc_blockactiontest.cpp"
