/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "troubleshootsettingswidgettest.h"
#include "administratorsettingsdialog/troubleshoot/troubleshootsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QFormLayout>
#include <QTest>

QTEST_MAIN(TroubleshootSettingsWidgetTest)
TroubleshootSettingsWidgetTest::TroubleshootSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void TroubleshootSettingsWidgetTest::shouldHaveDefaultValues()
{
    TroubleshootSettingsWidget w(nullptr);
    // TODO
}
