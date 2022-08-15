/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managedevicedialogtest.h"
#include "managedevice/managedevicedialog.h"
#include "managedevice/managedevicewidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ManageDeviceDialogTest)
ManageDeviceDialogTest::ManageDeviceDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ManageDeviceDialogTest::shouldHaveDefaultValues()
{
    ManageDeviceDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mManageDeviceWidget = d.findChild<ManageDeviceWidget *>(QStringLiteral("mManageDeviceWidget"));
    QVERIFY(mManageDeviceWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}
