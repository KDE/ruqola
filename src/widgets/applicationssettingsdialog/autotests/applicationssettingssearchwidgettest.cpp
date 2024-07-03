/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingssearchwidgettest.h"
#include "applicationssettingsdialog/applicationssettingscategoriescombobox.h"
#include "applicationssettingsdialog/applicationssettingsinstalledcombobox.h"
#include "applicationssettingsdialog/applicationssettingspricecombobox.h"
#include "applicationssettingsdialog/applicationssettingssearchwidget.h"
#include "applicationssettingsdialog/applicationssettingssortingcombobox.h"
#include "applicationssettingsdialog/applicationssettingsstatuscombobox.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(ApplicationsSettingsSearchWidgetTest)
ApplicationsSettingsSearchWidgetTest::ApplicationsSettingsSearchWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsSearchWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsSearchWidget d(nullptr);
    auto mainLayout = d.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mSearchLineEdit = d.findChild<QLineEdit *>(QStringLiteral("mSearchLineEdit"));
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->text().isEmpty());

    auto mApplicationsSettingsCategoriesComboBox =
        d.findChild<ApplicationsSettingsCategoriesComboBox *>(QStringLiteral("mApplicationsSettingsCategoriesComboBox"));
    QVERIFY(mApplicationsSettingsCategoriesComboBox);

    auto mApplicationsSettingsPriceComboBox = d.findChild<ApplicationsSettingsPriceComboBox *>(QStringLiteral("mApplicationsSettingsPriceComboBox"));
    QVERIFY(mApplicationsSettingsPriceComboBox);

    auto mApplicationsSettingsStatusComboBox = d.findChild<ApplicationsSettingsStatusComboBox *>(QStringLiteral("mApplicationsSettingsStatusComboBox"));
    QVERIFY(mApplicationsSettingsStatusComboBox);

    auto mApplicationsSettingsSortingComboBox = d.findChild<ApplicationsSettingsSortingComboBox *>(QStringLiteral("mApplicationsSettingsSortingComboBox"));
    QVERIFY(mApplicationsSettingsSortingComboBox);

    auto mApplicationsSettingsInstalledComboBox =
        d.findChild<ApplicationsSettingsInstalledComboBox *>(QStringLiteral("mApplicationsSettingsInstalledComboBox"));
    QVERIFY(mApplicationsSettingsInstalledComboBox);
}

#include "moc_applicationssettingssearchwidgettest.cpp"
