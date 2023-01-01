/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configuresettingsdialogtest.h"
#include "configuredialog/configuresettingsdialog.h"
#include <QStandardPaths>
#include <QTest>

QTEST_MAIN(ConfigureSettingsDialogTest)
ConfigureSettingsDialogTest::ConfigureSettingsDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ConfigureSettingsDialogTest::shouldHaveDefaultValues()
{
    ConfigureSettingsDialog w;
    QVERIFY(!w.windowTitle().isEmpty());
    // TODO (add more)
}
