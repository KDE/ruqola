/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingsdescriptiondialogtest.h"
#include "applicationssettingsdialog/applicationssettingsdescriptiondialog.h"
#include "applicationssettingsdialog/applicationssettingsdescriptiontabwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ApplicationsSettingsDescriptionDialogTest)
ApplicationsSettingsDescriptionDialogTest::ApplicationsSettingsDescriptionDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ApplicationsSettingsDescriptionDialogTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsDescriptionDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
    auto mApplicationsSettingsDescriptionWidget =
        d.findChild<ApplicationsSettingsDescriptionTabWidget *>(QStringLiteral("mApplicationsSettingsDescriptionWidget"));
    QVERIFY(mApplicationsSettingsDescriptionWidget);
    QCOMPARE(button->standardButtons(), QDialogButtonBox::Close);
}

#include "moc_applicationssettingsdescriptiondialogtest.cpp"
