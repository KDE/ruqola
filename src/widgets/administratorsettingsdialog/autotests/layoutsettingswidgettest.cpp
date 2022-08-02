/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "layoutsettingswidgettest.h"
#include "administratorsettingsdialog/layout/layoutsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QFormLayout>
#include <QTest>
QTEST_MAIN(LayoutSettingsWidgetTest)

LayoutSettingsWidgetTest::LayoutSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void LayoutSettingsWidgetTest::shouldHaveDefaultValues()
{
}
