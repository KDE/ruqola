/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appcountinfowidgettest.h"
#include "applicationssettingsdialog/appcountinfowidget.h"
#include <QHBoxLayout>
#include <QTest>

QTEST_MAIN(AppCountInfoWidgetTest)

AppCountInfoWidgetTest::AppCountInfoWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AppCountInfoWidgetTest::shouldHaveDefaultValues()
{
    AppCountInfoWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    // TODO
}

#include "moc_appcountinfowidgettest.cpp"
