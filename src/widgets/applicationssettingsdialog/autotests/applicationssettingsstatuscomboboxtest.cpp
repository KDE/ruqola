/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsstatuscomboboxtest.h"
#include "applicationssettingsdialog/applicationssettingsstatuscombobox.h"
#include <QTest>
QTEST_MAIN(ApplicationsSettingsStatusComboBoxTest)
ApplicationsSettingsStatusComboBoxTest::ApplicationsSettingsStatusComboBoxTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsStatusComboBoxTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsStatusComboBox d;
    QVERIFY(d.currentStatus().isEmpty());
}

#include "moc_applicationssettingsstatuscomboboxtest.cpp"
