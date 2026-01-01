/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

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
    QCOMPARE(d.currentStatus(), AppsMarketPlaceFilterProxyModel::Status::AllStatus);
}

#include "moc_applicationssettingsstatuscomboboxtest.cpp"
