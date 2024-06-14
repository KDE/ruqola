/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingsdescriptionwidgettest.h"
#include "applicationssettingsdialog/applicationssettingsdescriptionwidget.h"

#include <QTest>
QTEST_MAIN(ApplicationsSettingsDescriptionWidgetTest)
ApplicationsSettingsDescriptionWidgetTest::ApplicationsSettingsDescriptionWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsDescriptionWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsDescriptionWidget d;
    // TODO
}

#include "moc_applicationssettingsdescriptionwidgettest.cpp"
