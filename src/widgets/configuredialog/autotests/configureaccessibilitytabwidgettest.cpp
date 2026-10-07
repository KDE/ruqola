/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "configureaccessibilitytabwidgettest.h"

#include "configuredialog/configureaccessibilitytabwidget.h"
#include <QTest>
QTEST_MAIN(ConfigureAccessibilityTabWidgetTest)

ConfigureAccessibilityTabWidgetTest::ConfigureAccessibilityTabWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConfigureAccessibilityTabWidgetTest::shouldHaveDefaultValues()
{
    const ConfigureAccessibilityTabWidget w;
    QVERIFY(w.tabBarAutoHide());
}

#include "moc_configureaccessibilitytabwidgettest.cpp"
