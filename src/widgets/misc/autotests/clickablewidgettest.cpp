/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "clickablewidgettest.h"
#include "misc/clickablewidget.h"
#include <QTest>
QTEST_MAIN(ClickableWidgetTest)
ClickableWidgetTest::ClickableWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ClickableWidgetTest::shouldHaveDefaultValues()
{
    ClickableWidget w(QStringLiteral("foo"));
    QVERIFY(!w.name().isEmpty()); // See foo in constructor
    QVERIFY(w.identifier().isEmpty());
}

#include "moc_clickablewidgettest.cpp"
