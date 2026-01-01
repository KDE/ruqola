/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewtest.h"
#include "autogenerateui/autogenerateinteractionuiview.h"
#include <QTest>
QTEST_MAIN(AutoGenerateInteractionUiViewTest)

AutoGenerateInteractionUiViewTest::AutoGenerateInteractionUiViewTest(QObject *parent)
    : QObject{parent}
{
}

void AutoGenerateInteractionUiViewTest::shouldHaveDefaultValues()
{
    AutoGenerateInteractionUiView w;
    QVERIFY(!w.showIcon());
    QVERIFY(w.id().isEmpty());
    QVERIFY(!w.closeButton());
    QVERIFY(!w.submitButton());
    // TODO
}

#include "moc_autogenerateinteractionuiviewtest.cpp"
