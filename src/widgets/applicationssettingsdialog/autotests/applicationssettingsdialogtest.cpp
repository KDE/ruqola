/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdialogtest.h"
#include "applicationssettingsdialog/applicationssettingsdialog.h"
#include "applicationssettingsdialog/applicationssettingswidgetbase.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ApplicationsSettingsDialogTest)
ApplicationsSettingsDialogTest::ApplicationsSettingsDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ApplicationsSettingsDialogTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mBannerInfoWidget = d.findChild<ApplicationsSettingsWidgetBase *>(QStringLiteral("mApplicationsSettingsWidget"));
    QVERIFY(mBannerInfoWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_applicationssettingsdialogtest.cpp"
