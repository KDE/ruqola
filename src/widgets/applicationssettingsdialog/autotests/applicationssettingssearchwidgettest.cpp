/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingssearchwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "applicationssettingsdialog/applicationssettingscategoriescombobox.h"
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
    auto mainLayout = d.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mSearchLineEdit = d.findChild<QLineEdit *>(u"mSearchLineEdit"_s);
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->text().isEmpty());

    auto mApplicationsSettingsCategoriesComboBox = d.findChild<ApplicationsSettingsCategoriesComboBox *>(u"mApplicationsSettingsCategoriesComboBox"_s);
    QVERIFY(mApplicationsSettingsCategoriesComboBox);

    auto mApplicationsSettingsPriceComboBox = d.findChild<ApplicationsSettingsPriceComboBox *>(u"mApplicationsSettingsPriceComboBox"_s);
    QVERIFY(mApplicationsSettingsPriceComboBox);

    auto mApplicationsSettingsStatusComboBox = d.findChild<ApplicationsSettingsStatusComboBox *>(u"mApplicationsSettingsStatusComboBox"_s);
    QVERIFY(mApplicationsSettingsStatusComboBox);

    auto mApplicationsSettingsSortingComboBox = d.findChild<ApplicationsSettingsSortingComboBox *>(u"mApplicationsSettingsSortingComboBox"_s);
    QVERIFY(mApplicationsSettingsSortingComboBox);
}

#include "moc_applicationssettingssearchwidgettest.cpp"
