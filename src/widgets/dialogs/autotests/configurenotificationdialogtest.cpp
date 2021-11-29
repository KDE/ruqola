/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "configurenotificationdialogtest.h"
#include "dialogs/configurenotificationdialog.h"
#include "dialogs/configurenotificationwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfigureNotificationDialogTest)
ConfigureNotificationDialogTest::ConfigureNotificationDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ConfigureNotificationDialogTest::shouldHaveDefaultValues()
{
    ConfigureNotificationDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mConfigureNoticationWidget = w.findChild<ConfigureNotificationWidget *>(QStringLiteral("mConfigureNoticationWidget"));
    QVERIFY(mConfigureNoticationWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("buttonBox"));
    QVERIFY(buttonBox);
}
