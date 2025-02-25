/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewoverflowelementtest.h"
#include "autogenerateui/autogenerateinteractionuiviewoverflowelement.h"
#include <QTest>
QTEST_MAIN(AutoGenerateInteractionUiViewOverflowElementTest)

AutoGenerateInteractionUiViewOverflowElementTest::AutoGenerateInteractionUiViewOverflowElementTest(QObject *parent)
    : QObject{parent}
{
}

void AutoGenerateInteractionUiViewOverflowElementTest::shouldHaveDefaultValues()
{
    AutoGenerateInteractionUiViewOverflowElement f;
    // TODO
}

#include "moc_autogenerateinteractionuiviewoverflowelementtest.cpp"
