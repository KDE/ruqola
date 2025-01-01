/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscountinfowidgettest.h"
#include "applicationssettingsdialog/appscountinfowidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
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

    auto mProgressBar = w.findChild<QProgressBar *>(QStringLiteral("mProgressBar"));
    QVERIFY(mProgressBar);

    auto mApplicationInfo = w.findChild<QLabel *>(QStringLiteral("mApplicationInfo"));
    QVERIFY(mApplicationInfo);
    QVERIFY(mApplicationInfo->text().isEmpty());

    auto mEnableAppsLabel = w.findChild<QLabel *>(QStringLiteral("mEnableAppsLabel"));
    QVERIFY(mEnableAppsLabel);
    QVERIFY(mEnableAppsLabel->text().isEmpty());

    QCOMPARE(w.infotype(), AppsCountInfoWidget::InfoType::Unknown);
}

#include "moc_appscountinfowidgettest.cpp"
