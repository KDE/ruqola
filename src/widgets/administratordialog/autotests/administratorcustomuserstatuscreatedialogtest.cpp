/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomuserstatuscreatedialogtest.h"
#include "administratordialog/customuserstatus/administratorcustomuserstatuscreatedialog.h"
#include "administratordialog/customuserstatus/administratorcustomuserstatuscreatewidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorCustomUserStatusCreateDialogTest)

AdministratorCustomUserStatusCreateDialogTest::AdministratorCustomUserStatusCreateDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void AdministratorCustomUserStatusCreateDialogTest::shouldHaveDefaultValues()
{
    AdministratorCustomUserStatusCreateDialog w;
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mCreateWidget = w.findChild<AdministratorCustomUserStatusCreateWidget *>(QStringLiteral("mCreateWidget"));
    QVERIFY(mCreateWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);

    QVERIFY(!w.userStatusInfo().isValid());
}
