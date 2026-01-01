/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingssortingcomboboxtest.h"
#include "applicationssettingsdialog/applicationssettingssortingcombobox.h"
#include <QTest>

QTEST_MAIN(ApplicationsSettingsSortingComboBoxTest)
ApplicationsSettingsSortingComboBoxTest::ApplicationsSettingsSortingComboBoxTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsSortingComboBoxTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsSortingComboBox d;
    QCOMPARE(d.currentSorting(), AppsMarketPlaceFilterProxyModel::Sorting::AtoZ);
}

#include "moc_applicationssettingssortingcomboboxtest.cpp"
