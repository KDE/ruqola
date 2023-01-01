/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userinroleeditdialogtest.h"
#include "administratordialog/roles/userinroleeditdialog.h"
#include "administratordialog/roles/usersinrolewidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(UserInRoleEditDialogTest)
UserInRoleEditDialogTest::UserInRoleEditDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void UserInRoleEditDialogTest::shouldHaveDefaultValues()
{
    UserInRoleEditDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());
    QVERIFY(d.roleId().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    auto mUserInRoleEditWidget = d.findChild<UsersInRoleWidget *>(QStringLiteral("mUserInRoleEditWidget"));
    QVERIFY(mUserInRoleEditWidget);
}
