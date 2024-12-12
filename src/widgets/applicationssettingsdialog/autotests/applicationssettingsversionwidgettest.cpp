/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingsversionwidgettest.h"
#include "applicationssettingsdialog/applicationssettingsversionwidget.h"

#include <QTest>
#include <QTextBrowser>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(ApplicationsSettingsVersionWidgetTest)
ApplicationsSettingsVersionWidgetTest::ApplicationsSettingsVersionWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsVersionWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsVersionWidget d;

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mTextBrowser = d.findChild<QTextBrowser *>("mTextBrowser"_L1);
    QVERIFY(mTextBrowser);
    QVERIFY(mTextBrowser->openExternalLinks());
}

#include "moc_applicationssettingsversionwidgettest.cpp"
