/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewplaintextinputelementtest.h"
#include "autogenerateui/autogenerateinteractionuiviewplaintextinputelement.h"
#include <QTest>

QTEST_MAIN(AutoGenerateInteractionUiViewPlainTextInputElementTest)

AutoGenerateInteractionUiViewPlainTextInputElementTest::AutoGenerateInteractionUiViewPlainTextInputElementTest(QObject *parent)
    : QObject{parent}
{
}

void AutoGenerateInteractionUiViewPlainTextInputElementTest::shouldHaveDefaultValues()
{
    AutoGenerateInteractionUiViewPlainTextInputElement e;
    QVERIFY(!e.placeHolder());
    QVERIFY(e.initialValue().isEmpty());
    QVERIFY(!e.multiLine());
    QCOMPARE(e.minLength(), -1);
    QCOMPARE(e.maxLength(), -1);
}

#include "moc_autogenerateinteractionuiviewplaintextinputelementtest.cpp"
