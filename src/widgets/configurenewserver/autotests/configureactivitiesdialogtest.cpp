/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureactivitiesdialogtest.h"
#include "configurenewserver/configureactivitiesdialog.h"
#include "configurenewserver/configureactivitieswidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfigureActivitiesDialogTest)
ConfigureActivitiesDialogTest::ConfigureActivitiesDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ConfigureActivitiesDialogTest::shouldHaveDefaultValues()
{
    ConfigureActivitiesDialog d;
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mConfigureActivitiesWidget = d.findChild<ConfigureActivitiesWidget *>(QStringLiteral("mConfigureActivitiesWidget"));
    QVERIFY(mConfigureActivitiesWidget);

    auto buttonBox = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
}

#include "moc_configureactivitiesdialogtest.cpp"
