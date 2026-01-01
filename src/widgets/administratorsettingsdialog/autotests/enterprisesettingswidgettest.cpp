/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "enterprisesettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/enterprise/enterprisesettingswidget.h"
#include "settingswidgetshelper.h"
#include <QLabel>
#include <QLineEdit>
QTEST_MAIN(EnterpriseSettingsWidgetTest)

EnterpriseSettingsWidgetTest::EnterpriseSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void EnterpriseSettingsWidgetTest::shouldHaveDefaultValues()
{
    EnterpriseSettingsWidget w(nullptr);
    auto mEnterpriseLicense = w.findChild<QLineEdit *>(u"mEnterpriseLicense"_s);
    QVERIFY(mEnterpriseLicense);
    QVERIFY(mEnterpriseLicense->text().isEmpty());
    QVERIFY(!mEnterpriseLicense->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnterpriseLicense), u"Enterprise_License"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Enterprise_License"_s);

    auto mStatus = w.findChild<QLabel *>(u"mStatus"_s);
    QVERIFY(mStatus);
    QVERIFY(mStatus->text().isEmpty());
    QVERIFY(mStatus->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mStatus), u"Enterprise_License_Status"_s);
}

#include "moc_enterprisesettingswidgettest.cpp"
