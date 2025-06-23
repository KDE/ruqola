/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "applicationssettingsdialog/applicationssettingsdialog.h"
#include "applicationssettingsdialog/applicationssettingswidget.h"
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
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mBannerInfoWidget = d.findChild<ApplicationsSettingsWidget *>(u"mApplicationsSettingsWidget"_s);
    QVERIFY(mBannerInfoWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_applicationssettingsdialogtest.cpp"
