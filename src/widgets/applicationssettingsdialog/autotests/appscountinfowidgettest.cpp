/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscountinfowidgettest.h"
#include "applicationssettingsdialog/appscountinfowidget.h"
#include <QHBoxLayout>
#include <QTest>

QTEST_MAIN(AppsCountInfoWidgetTest)

AppsCountInfoWidgetTest::AppsCountInfoWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AppsCountInfoWidgetTest::shouldHaveDefaultValues()
{
    AppsCountInfoWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    // TODO
}

#include "moc_appscountinfowidgettest.cpp"
