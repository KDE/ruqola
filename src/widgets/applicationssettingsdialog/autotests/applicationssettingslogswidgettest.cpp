/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingslogswidgettest.h"
#include "applicationssettingsdialog/applicationssettingslogswidget.h"

#include <QTest>
#include <QTextBrowser>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(ApplicationsSettingsLogsWidgetTest)
ApplicationsSettingsLogsWidgetTest::ApplicationsSettingsLogsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsLogsWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsLogsWidget d(nullptr);

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mTextBrowser = d.findChild<QTextBrowser *>("mTextBrowser"_L1);
    QVERIFY(mTextBrowser);
    QVERIFY(mTextBrowser->openExternalLinks());
}

#include "moc_applicationssettingslogswidgettest.cpp"
