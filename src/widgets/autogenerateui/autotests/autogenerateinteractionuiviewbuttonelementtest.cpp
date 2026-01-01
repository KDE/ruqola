/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewbuttonelementtest.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewbuttonelement.h"
#include <QTest>
QTEST_MAIN(AutoGenerateInteractionUiViewButtonElementTest)

AutoGenerateInteractionUiViewButtonElementTest::AutoGenerateInteractionUiViewButtonElementTest(QObject *parent)
    : QObject{parent}
{
}

void AutoGenerateInteractionUiViewButtonElementTest::shouldHaveDefaultValues()
{
    AutoGenerateInteractionUiViewButtonElement b;
    QVERIFY(!b.secondary());
    // TODO
}

#include "moc_autogenerateinteractionuiviewbuttonelementtest.cpp"
