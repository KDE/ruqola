/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsinprogresswidgettest.h"
#include "applicationssettingsdialog/applicationssettingsinprogresswidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(ApplicationsSettingsInProgressWidgetTest)
ApplicationsSettingsInProgressWidgetTest::ApplicationsSettingsInProgressWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsInProgressWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsInProgressWidget d;
    auto mainLayout = d.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mLabel = d.findChild<QLabel *>("label"_L1);
    QVERIFY(mLabel);
    QVERIFY(!mLabel->text().isEmpty());

    auto mProgressBar = d.findChild<QProgressBar *>("mProgressBar"_L1);
    QVERIFY(mProgressBar);
    QCOMPARE(mProgressBar->value(), 0);
    QCOMPARE(mProgressBar->minimum(), 0);
    QCOMPARE(mProgressBar->maximum(), 0);
}

#include "moc_applicationssettingsinprogresswidgettest.cpp"
