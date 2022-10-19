/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "logssettingswidgettest.h"
#include "administratorsettingsdialog/logs/logssettingswidget.h"
#include <QTest>
QTEST_MAIN(LogsSettingsWidgetTest)
LogsSettingsWidgetTest::LogsSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void LogsSettingsWidgetTest::shouldHaveDefaultValues()
{
    LogsSettingsWidget w(nullptr);
    // TODO
}
