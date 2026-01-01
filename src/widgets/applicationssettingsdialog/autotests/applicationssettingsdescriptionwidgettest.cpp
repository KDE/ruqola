/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingsdescriptionwidgettest.h"
#include "applicationssettingsdialog/applicationssettingsdescriptionwidget.h"

#include <QTest>
#include <QTextBrowser>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(ApplicationsSettingsDescriptionWidgetTest)
ApplicationsSettingsDescriptionWidgetTest::ApplicationsSettingsDescriptionWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsDescriptionWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsDescriptionWidget d;

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mTextBrowser = d.findChild<QTextBrowser *>("mTextBrowser"_L1);
    QVERIFY(mTextBrowser);
    QVERIFY(mTextBrowser->openExternalLinks());
}

#include "moc_applicationssettingsdescriptionwidgettest.cpp"
