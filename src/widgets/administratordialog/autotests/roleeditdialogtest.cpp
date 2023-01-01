/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roleeditdialogtest.h"
#include "administratordialog/roles/roleeditdialog.h"
#include "administratordialog/roles/roleeditwidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(RoleEditDialogTest)
RoleEditDialogTest::RoleEditDialogTest(QObject *parent)
    : QObject{parent}
{
}

void RoleEditDialogTest::shouldHaveDefaultValues()
{
    RoleEditDialog d;
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    auto mRoleEditWidget = d.findChild<RoleEditWidget *>(QStringLiteral("mRoleEditWidget"));
    QVERIFY(mRoleEditWidget);

    QVERIFY(!d.roleEditDialogInfo().isValid());
}
