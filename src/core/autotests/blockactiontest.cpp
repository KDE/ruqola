/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blockactiontest.h"
<<<<<<< HEAD
=======
#include "messages/blockaction.h"
    >>>>>>> origin
        / 2.1
#include <QTest>
        QTEST_GUILESS_MAIN(BlockActionTest)

            BlockActionTest::BlockActionTest(QObject *parent)
    : QObject{parent}
{
}
<<<<<<< HEAD
=======

void BlockActionTest::shouldHaveDefaultValues()
{
    BlockAction b;
    QVERIFY(b.actionId().isEmpty());
    QVERIFY(b.text().isEmpty());
    QVERIFY(b.type().isEmpty());
}

#include "moc_blockactiontest.cpp"
>>>>>>> origin/2.1
