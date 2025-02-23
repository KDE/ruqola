/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuitest.h"
#include "autogenerateui/autogenerateinteractionui.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>

QTEST_GUILESS_MAIN(AutoGenerateInteractionUiTest)

AutoGenerateInteractionUiTest::AutoGenerateInteractionUiTest(QObject *parent)
    : QObject{parent}
{
}

void AutoGenerateInteractionUiTest::shouldHaveDefaultValues()
{
    AutoGenerateInteractionUi ui;
    QVERIFY(ui.appId().isEmpty());
    QVERIFY(ui.triggerId().isEmpty());
    QCOMPARE(ui.typeUi(), AutoGenerateInteractionUi::TypeUi::Unknown);
}

#include "moc_autogenerateinteractionuitest.cpp"
