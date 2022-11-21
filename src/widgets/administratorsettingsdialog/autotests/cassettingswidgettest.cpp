/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "cassettingswidgettest.h"
#include "administratorsettingsdialog/cas/cassettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QTest>
QTEST_MAIN(CasSettingsWidgetTest)
CasSettingsWidgetTest::CasSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CasSettingsWidgetTest::shouldHaveDefaultValues()
{
    CasSettingsWidget w(nullptr);
    auto mEnabled = w.findChild<QCheckBox *>(QStringLiteral("mEnabled"));
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), QStringLiteral("CAS_enabled"));
}
