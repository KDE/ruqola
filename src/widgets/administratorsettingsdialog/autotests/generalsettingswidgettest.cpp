/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "generalsettingswidgettest.h"
#include "administratorsettingsdialog/general/generalsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(GeneralSettingsWidgetTest)
GeneralSettingsWidgetTest::GeneralSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void GeneralSettingsWidgetTest::shouldHaveDefaultValues()
{
    GeneralSettingsWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mEnableFavoriteRooms = w.findChild<QCheckBox *>(QStringLiteral("mEnableFavoriteRooms"));
    QVERIFY(mEnableFavoriteRooms);
    QVERIFY(!mEnableFavoriteRooms->isChecked());
    QVERIFY(!mEnableFavoriteRooms->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableFavoriteRooms), QStringLiteral("Favorite_Rooms"));

    auto mSiteUrl = w.findChild<QLineEdit *>(QStringLiteral("mSiteUrl"));
    QVERIFY(mSiteUrl);
    QVERIFY(mSiteUrl->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSiteUrl), QStringLiteral("Site_Url"));
}
