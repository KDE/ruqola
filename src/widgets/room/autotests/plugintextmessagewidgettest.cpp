/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

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
    w.show();
    QVERIFY(w.text().isEmpty());
    QVERIFY(w.isCloseButtonVisible());
}

#include "moc_plugintextmessagewidgettest.cpp"
