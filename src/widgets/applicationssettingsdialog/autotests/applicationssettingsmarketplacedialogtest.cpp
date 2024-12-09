/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsmarketplacedialogtest.h"
#include "applicationssettingsdialog/applicationssettingsmarketplacedialog.h"
#include "applicationssettingsdialog/applicationssettingswidgetbase.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ApplicationsSettingsMarketPlaceDialogTest)
ApplicationsSettingsMarketPlaceDialogTest::ApplicationsSettingsMarketPlaceDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ApplicationsSettingsMarketPlaceDialogTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsMarketPlaceDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mBannerInfoWidget = d.findChild<ApplicationsSettingsWidgetBase *>(QStringLiteral("mApplicationsSettingsWidget"));
    QVERIFY(mBannerInfoWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_applicationssettingsmarketplacedialogtest.cpp"
