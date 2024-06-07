/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdialogtest.h"
#include "applicationssettingsdialog/applicationssettingsdialog.h"
#include <QTest>

QTEST_MAIN(ApplicationsSettingsDialogTest)
ApplicationsSettingsDialogTest::ApplicationsSettingsDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsDialogTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsDialog d(nullptr);
    // TODO
}

#include "moc_applicationssettingsdialogtest.cpp"
