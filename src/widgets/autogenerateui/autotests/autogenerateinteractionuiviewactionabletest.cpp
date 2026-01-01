/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewactionabletest.h"
#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include <QTest>
QTEST_MAIN(AutoGenerateInteractionUiViewActionableTest)

AutoGenerateInteractionUiViewActionableTest::AutoGenerateInteractionUiViewActionableTest(QObject *parent)
    : QObject{parent}
{
}

void AutoGenerateInteractionUiViewActionableTest::shouldHaveDefaultValues()
{
    /*
    AutoGenerateInteractionUiViewActionable a;
    QVERIFY(a.appId().isEmpty());
    QVERIFY(a.blockId().isEmpty());
    QVERIFY(a.actionId().isEmpty());
    */
}

#include "moc_autogenerateinteractionuiviewactionabletest.cpp"
