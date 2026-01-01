/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewdatepickerelementtest.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewdatepickerelement.h"
#include <QTest>
QTEST_MAIN(AutoGenerateInteractionUiViewDatePickerElementTest)

AutoGenerateInteractionUiViewDatePickerElementTest::AutoGenerateInteractionUiViewDatePickerElementTest(QObject *parent)
    : QObject{parent}
{
}

void AutoGenerateInteractionUiViewDatePickerElementTest::shouldHaveDefaultValues()
{
    AutoGenerateInteractionUiViewDatePickerElement e;
    QVERIFY(e.initialDate().isEmpty());
    // TODO
}

#include "moc_autogenerateinteractionuiviewdatepickerelementtest.cpp"
