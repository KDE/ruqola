/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userinroleeditdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    auto mUserInRoleEditWidget = d.findChild<UsersInRoleWidget *>(u"mUserInRoleEditWidget"_s);
    QVERIFY(mUserInRoleEditWidget);
}

#include "moc_userinroleeditdialogtest.cpp"
