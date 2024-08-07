/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blockstest.h"
#include "messages/blocks.h"
#include <QTest>
QTEST_MAIN(BlocksTest)
BlocksTest::BlocksTest(QObject *parent)
    : QObject{parent}
{
}

void BlocksTest::shouldHaveDefaultValue()
{
    Blocks w;
    QVERIFY(w.isEmpty());
    QVERIFY(w.blocks().isEmpty());
}

#include "moc_blockstest.cpp"
