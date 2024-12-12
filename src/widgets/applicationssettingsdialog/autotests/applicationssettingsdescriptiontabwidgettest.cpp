/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingsdescriptiontabwidgettest.h"
#include "applicationssettingsdialog/applicationssettingsdescriptiontabwidget.h"
#include <QTest>

QTEST_MAIN(ApplicationsSettingsDescriptionTabWidgetTest)

ApplicationsSettingsDescriptionTabWidgetTest::ApplicationsSettingsDescriptionTabWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsDescriptionTabWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsDescriptionTabWidget w;
    // TODO
}

#include "moc_applicationssettingsdescriptiontabwidgettest.cpp"
