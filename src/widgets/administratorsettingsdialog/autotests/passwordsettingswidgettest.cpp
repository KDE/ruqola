/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordsettingswidgettest.h"
#include "administratorsettingsdialog/password/passwordsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QTest>
QTEST_MAIN(PasswordSettingsWidgetTest)
PasswordSettingsWidgetTest::PasswordSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void PasswordSettingsWidgetTest::shouldHaveDefaultValues()
{
    PasswordSettingsWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mEnablePasswordHistory = w.findChild<QCheckBox *>(QStringLiteral("mEnablePasswordHistory"));
    QVERIFY(mEnablePasswordHistory);
    QVERIFY(!mEnablePasswordHistory->isChecked());
    QVERIFY(!mEnablePasswordHistory->text().isEmpty());
    QVERIFY(!mEnablePasswordHistory->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnablePasswordHistory), QStringLiteral("Accounts_Password_History_Enabled"));
}
