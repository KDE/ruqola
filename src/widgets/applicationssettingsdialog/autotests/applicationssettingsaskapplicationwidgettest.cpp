/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsaskapplicationwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "applicationssettingsdialog/applicationssettingsaskapplicationwidget.h"
#include <KTextEdit>
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ApplicationsSettingsAskApplicationWidgetTest)
ApplicationsSettingsAskApplicationWidgetTest::ApplicationsSettingsAskApplicationWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsAskApplicationWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsAskApplicationWidget d;
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = d.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mTextAppLabel = d.findChild<QLabel *>(u"mTextAppLabel"_s);
    QVERIFY(mTextAppLabel);
    QVERIFY(mTextAppLabel->text().isEmpty());

    auto mMessage = d.findChild<KTextEdit *>(u"mMessage"_s);
    QVERIFY(mMessage);
    QVERIFY(mMessage->toPlainText().isEmpty());
    QVERIFY(!mMessage->placeholderText().isEmpty());
}

#include "moc_applicationssettingsaskapplicationwidgettest.cpp"
