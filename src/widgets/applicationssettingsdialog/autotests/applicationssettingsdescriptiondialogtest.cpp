/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingsdescriptiondialogtest.h"
#include "applicationssettingsdialog/applicationssettingsdescriptiondialog.h"
#include <QTest>
QTEST_MAIN(ApplicationsSettingsDescriptionDialogTest)
ApplicationsSettingsDescriptionDialogTest::ApplicationsSettingsDescriptionDialogTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsDescriptionDialogTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsDescriptionDialog d;
    // TODO
}
