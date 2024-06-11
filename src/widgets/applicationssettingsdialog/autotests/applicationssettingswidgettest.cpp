/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingswidgettest.h"
#include "applicationssettingsdialog/applicationssettingswidget.h"
#include <QTest>

QTEST_MAIN(ApplicationsSettingsWidgetTest)
ApplicationsSettingsWidgetTest::ApplicationsSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsWidget d(nullptr);

    // TODO
}

#include "moc_applicationssettingswidgettest.cpp"
