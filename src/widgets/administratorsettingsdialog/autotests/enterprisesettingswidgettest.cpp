/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "enterprisesettingswidgettest.h"
#include "administratorsettingsdialog/enterprise/enterprisesettingswidget.h"
#include "settingswidgetshelper.h"
#include <QLabel>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(EnterpriseSettingsWidgetTest)

EnterpriseSettingsWidgetTest::EnterpriseSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void EnterpriseSettingsWidgetTest::shouldHaveDefaultValues()
{
    EnterpriseSettingsWidget w(nullptr);
    auto mEnterpriseLicense = w.findChild<QLineEdit *>(QStringLiteral("mEnterpriseLicense"));
    QVERIFY(mEnterpriseLicense);
    QVERIFY(mEnterpriseLicense->text().isEmpty());
    QVERIFY(!mEnterpriseLicense->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnterpriseLicense), QStringLiteral("Enterprise_License"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Enterprise_License"));

    auto mStatus = w.findChild<QLabel *>(QStringLiteral("mStatus"));
    QVERIFY(mStatus);
    QVERIFY(mStatus->text().isEmpty());
    QVERIFY(mStatus->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mStatus), QStringLiteral("Enterprise_License_Status"));
}
