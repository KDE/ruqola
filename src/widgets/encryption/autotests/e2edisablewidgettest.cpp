/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edisablewidgettest.h"
#include "encryption/e2edisablewidget.h"
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(E2eDisableWidgetTest)

E2eDisableWidgetTest::E2eDisableWidgetTest(QObject *parent)
    : QObject{parent}
{
}

E2eDisableWidgetTest::~E2eDisableWidgetTest() = default;

void E2eDisableWidgetTest::shouldHaveDefaultValues()
{
    E2eDisableWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
}

#include "moc_e2edisablewidgettest.cpp"
