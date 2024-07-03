/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsinstalledcomboboxtest.h"
#include "applicationssettingsdialog/applicationssettingsinstalledcombobox.h"

#include <QTest>
QTEST_MAIN(ApplicationsSettingsInstalledComboBoxTest)
ApplicationsSettingsInstalledComboBoxTest::ApplicationsSettingsInstalledComboBoxTest(QObject *parent)
    : QObject{parent}
{
}

ApplicationsSettingsInstalledComboBoxTest::~ApplicationsSettingsInstalledComboBoxTest() = default;

void ApplicationsSettingsInstalledComboBoxTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsInstalledComboBox d;
    // TODO
}

#include "moc_applicationssettingsinstalledcomboboxtest.cpp"
