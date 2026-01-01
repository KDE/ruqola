/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurenotificationdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mConfigureNoticationWidget = w.findChild<ConfigureNotificationWidget *>(u"mConfigureNoticationWidget"_s);
    QVERIFY(mConfigureNoticationWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(u"buttonBox"_s);
    QVERIFY(buttonBox);
}

#include "moc_configurenotificationdialogtest.cpp"
