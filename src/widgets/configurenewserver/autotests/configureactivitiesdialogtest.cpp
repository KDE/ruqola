/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureactivitiesdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mConfigureActivitiesWidget = d.findChild<ConfigureActivitiesWidget *>(u"mConfigureActivitiesWidget"_s);
    QVERIFY(mConfigureActivitiesWidget);

    auto buttonBox = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(buttonBox);
}

#include "moc_configureactivitiesdialogtest.cpp"
