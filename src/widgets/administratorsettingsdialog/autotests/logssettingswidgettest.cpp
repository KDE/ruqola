/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "logssettingswidgettest.h"
#include "administratorsettingsdialog/logs/logssettingswidget.h"
#include "settingswidgetshelper.h"
#include <QLineEdit>
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
}
