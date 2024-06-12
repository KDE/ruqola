/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsaskapplicationwidgettest.h"
#include "applicationssettingsdialog/applicationssettingsaskapplicationwidget.h"
#include <QTest>
QTEST_MAIN(ApplicationsSettingsAskApplicationWidgetTest)
ApplicationsSettingsAskApplicationWidgetTest::ApplicationsSettingsAskApplicationWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsAskApplicationWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsAskApplicationWidget d;
    // TODO
}

#include "moc_applicationssettingsaskapplicationwidgettest.cpp"
