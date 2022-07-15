/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ldapsettingswidgettest.h"
#include "administratorsettingsdialog/ldap/ldapsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QTest>

QTEST_MAIN(LDapSettingsWidgetTest)
LDapSettingsWidgetTest::LDapSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void LDapSettingsWidgetTest::shouldHaveDefaultValues()
{
    LDapSettingsWidget w(nullptr);
    // TODO
}
