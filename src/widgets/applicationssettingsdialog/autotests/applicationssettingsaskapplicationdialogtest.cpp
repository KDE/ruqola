/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsaskapplicationdialogtest.h"
#include "applicationssettingsdialog/applicationssettingsaskapplicationdialog.h"
#include <QTest>
QTEST_MAIN(ApplicationsSettingsAskApplicationDialogTest)
ApplicationsSettingsAskApplicationDialogTest::ApplicationsSettingsAskApplicationDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsAskApplicationDialogTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsAskApplicationDialog d;
    // TODO
}

#include "moc_applicationssettingsaskapplicationdialogtest.cpp"
