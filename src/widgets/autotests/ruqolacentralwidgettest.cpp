/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolacentralwidgettest.h"
#include "ruqolacentralwidget.h"

#include <QHBoxLayout>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QTest>

QTEST_MAIN(RuqolaCentralWidgetTest)
RuqolaCentralWidgetTest::RuqolaCentralWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void RuqolaCentralWidgetTest::shouldHaveDefaultValues()
{
    RuqolaCentralWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);
    // TODO
}
