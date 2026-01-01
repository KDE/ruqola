/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "logssettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/logs/logssettingswidget.h"
#include "settingswidgetshelper.h"
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>

QTEST_MAIN(LogsSettingsWidgetTest)
LogsSettingsWidgetTest::LogsSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void LogsSettingsWidgetTest::shouldHaveDefaultValues()
{
    LogsSettingsWidget w(nullptr);
    auto mLogExceptionsChannel = w.findChild<QLineEdit *>(u"mLogExceptionsChannel"_s);
    QVERIFY(mLogExceptionsChannel);
    QVERIFY(mLogExceptionsChannel->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLogExceptionsChannel), u"Log_Exceptions_to_Channel"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Log_Exceptions_to_Channel"_s);

    auto mLogViewLimit = w.findChild<QSpinBox *>(u"mLogViewLimit"_s);
    QVERIFY(mLogViewLimit);
    QVERIFY(mLogViewLimit->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLogViewLimit), u"Log_View_Limit"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Log_View_Limit"_s);

    auto mLogLevel = w.findChild<QComboBox *>(u"mLogLevel"_s);
    QVERIFY(mLogLevel);
    QCOMPARE(mLogLevel->count(), 3);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLogLevel), u"Log_Level"_s);

    auto mTraceMethodCalls = w.findChild<QCheckBox *>(u"mTraceMethodCalls"_s);
    QVERIFY(mTraceMethodCalls);
    QVERIFY(!mTraceMethodCalls->isChecked());
    QVERIFY(!mTraceMethodCalls->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTraceMethodCalls), u"Log_Trace_Methods"_s);

    auto mTraceSubscriptionCalls = w.findChild<QCheckBox *>(u"mTraceSubscriptionCalls"_s);
    QVERIFY(mTraceSubscriptionCalls);
    QVERIFY(!mTraceSubscriptionCalls->isChecked());
    QVERIFY(!mTraceSubscriptionCalls->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTraceSubscriptionCalls), u"Log_Trace_Subscriptions"_s);

    auto mTraceMethodFilter = w.findChild<QLineEdit *>(u"mTraceMethodFilter"_s);
    QVERIFY(mTraceMethodFilter);
    QVERIFY(mTraceMethodFilter->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTraceMethodFilter), u"Log_Trace_Methods_Filter"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Log_Trace_Methods_Filter"_s);

    auto mTraceSubscriptionFilter = w.findChild<QLineEdit *>(u"mTraceSubscriptionFilter"_s);
    QVERIFY(mTraceSubscriptionFilter);
    QVERIFY(mTraceSubscriptionFilter->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTraceSubscriptionFilter), u"Log_Trace_Subscriptions_Filter"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Log_Trace_Subscriptions_Filter"_s);
}

#include "moc_logssettingswidgettest.cpp"
