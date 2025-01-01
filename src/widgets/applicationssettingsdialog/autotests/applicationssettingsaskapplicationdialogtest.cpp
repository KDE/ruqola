/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsaskapplicationdialogtest.h"
#include "applicationssettingsdialog/applicationssettingsaskapplicationdialog.h"
#include "applicationssettingsdialog/applicationssettingsaskapplicationwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ApplicationsSettingsAskApplicationDialogTest)
ApplicationsSettingsAskApplicationDialogTest::ApplicationsSettingsAskApplicationDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ApplicationsSettingsAskApplicationDialogTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsAskApplicationDialog d;
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
    auto mApplicationsSettingsAskApplicationWidget =
        d.findChild<ApplicationsSettingsAskApplicationWidget *>(QStringLiteral("mApplicationsSettingsAskApplicationWidget"));
    QVERIFY(mApplicationsSettingsAskApplicationWidget);
}

#include "moc_applicationssettingsaskapplicationdialogtest.cpp"
