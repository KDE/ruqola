/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plugintextmessagewidgettest.h"
#include "room/plugintextmessagewidget.h"
#include <QTest>

QTEST_MAIN(PluginTextMessageWidgetTest)
PluginTextMessageWidgetTest::PluginTextMessageWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void PluginTextMessageWidgetTest::shouldHaveDefaultValues()
{
    PluginTextMessageWidget w;
    QVERIFY(w.text().isEmpty());
    QVERIFY(!w.isVisible());
    // QVERIFY(w.isCloseButtonVisible());
}

#include "moc_plugintextmessagewidgettest.cpp"
