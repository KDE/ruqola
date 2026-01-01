/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingspricecomboboxtest.h"
#include "applicationssettingsdialog/applicationssettingspricecombobox.h"
#include <QTest>
QTEST_MAIN(ApplicationsSettingsPriceComboBoxTest)
ApplicationsSettingsPriceComboBoxTest::ApplicationsSettingsPriceComboBoxTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsPriceComboBoxTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsPriceComboBox d;
    QCOMPARE(d.currentPrice(), AppsMarketPlaceFilterProxyModel::Price::AllPrice);
}

#include "moc_applicationssettingspricecomboboxtest.cpp"
