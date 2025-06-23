/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsaskapplicationdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
    auto mApplicationsSettingsAskApplicationWidget = d.findChild<ApplicationsSettingsAskApplicationWidget *>(u"mApplicationsSettingsAskApplicationWidget"_s);
    QVERIFY(mApplicationsSettingsAskApplicationWidget);
}

#include "moc_applicationssettingsaskapplicationdialogtest.cpp"
