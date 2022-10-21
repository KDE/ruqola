/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "logssettingswidgettest.h"
#include "administratorsettingsdialog/logs/logssettingswidget.h"
#include "settingswidgetshelper.h"
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QTest>

QTEST_MAIN(LogsSettingsWidgetTest)
LogsSettingsWidgetTest::LogsSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void LogsSettingsWidgetTest::shouldHaveDefaultValues()
{
    LogsSettingsWidget w(nullptr);
    auto mLogExceptionsChannel = w.findChild<QLineEdit *>(QStringLiteral("mLogExceptionsChannel"));
    QVERIFY(mLogExceptionsChannel);
    QVERIFY(mLogExceptionsChannel->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLogExceptionsChannel), QStringLiteral("Log_Exceptions_to_Channel"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Log_Exceptions_to_Channel"));

    auto mLogViewLimit = w.findChild<QSpinBox *>(QStringLiteral("mLogViewLimit"));
    QVERIFY(mLogViewLimit);
    QVERIFY(mLogViewLimit->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLogViewLimit), QStringLiteral("Log_View_Limit"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Log_View_Limit"));

    auto mLogLevel = w.findChild<QComboBox *>(QStringLiteral("mLogLevel"));
    QVERIFY(mLogLevel);
    QCOMPARE(mLogLevel->count(), 3);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLogLevel), QStringLiteral("Log_Level"));

    auto mTraceMethodCalls = w.findChild<QCheckBox *>(QStringLiteral("mTraceMethodCalls"));
    QVERIFY(mTraceMethodCalls);
    QVERIFY(!mTraceMethodCalls->isChecked());
    QVERIFY(!mTraceMethodCalls->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTraceMethodCalls), QStringLiteral("Log_Trace_Methods"));

    auto mTraceSubscriptionCalls = w.findChild<QCheckBox *>(QStringLiteral("mTraceSubscriptionCalls"));
    QVERIFY(mTraceSubscriptionCalls);
    QVERIFY(!mTraceSubscriptionCalls->isChecked());
    QVERIFY(!mTraceSubscriptionCalls->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTraceSubscriptionCalls), QStringLiteral("Log_Trace_Subscriptions"));

    auto mTraceMethodFilter = w.findChild<QLineEdit *>(QStringLiteral("mTraceMethodFilter"));
    QVERIFY(mTraceMethodFilter);
    QVERIFY(mTraceMethodFilter->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTraceMethodFilter), QStringLiteral("Log_Trace_Methods_Filter"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Log_Trace_Methods_Filter"));

    auto mTraceSubscriptionFilter = w.findChild<QLineEdit *>(QStringLiteral("mTraceSubscriptionFilter"));
    QVERIFY(mTraceSubscriptionFilter);
    QVERIFY(mTraceSubscriptionFilter->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTraceSubscriptionFilter), QStringLiteral("Log_Trace_Subscriptions_Filter"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Log_Trace_Subscriptions_Filter"));
}
