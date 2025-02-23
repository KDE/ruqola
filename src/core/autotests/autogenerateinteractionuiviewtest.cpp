/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewtest.h"
#include "autogenerateui/autogenerateinteractionuiview.h"
#include <QTest>
QTEST_GUILESS_MAIN(AutoGenerateInteractionUiViewTest)

AutoGenerateInteractionUiViewTest::AutoGenerateInteractionUiViewTest(QObject *parent)
    : QObject{parent}
{
}

void AutoGenerateInteractionUiViewTest::shouldHaveDefaultValues()
{
    AutoGenerateInteractionUiView w;
    // TODO
}
