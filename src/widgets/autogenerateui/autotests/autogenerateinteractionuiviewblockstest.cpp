/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewblockstest.h"
#include "autogenerateui/autogenerateinteractionuiviewblocks.h"
#include <QTest>
QTEST_MAIN(AutoGenerateInteractionUiViewBlocksTest)

AutoGenerateInteractionUiViewBlocksTest::AutoGenerateInteractionUiViewBlocksTest(QObject *parent)
    : QObject{parent}
{
}

void AutoGenerateInteractionUiViewBlocksTest::shouldHaveDefaultValues()
{
    AutoGenerateInteractionUiViewBlocks b;
    QVERIFY(b.blockElements().isEmpty());
    // TODO
}

#include "moc_autogenerateinteractionuiviewblockstest.cpp"
