/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roleeditdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    auto mRoleEditWidget = d.findChild<RoleEditWidget *>(u"mRoleEditWidget"_s);
    QVERIFY(mRoleEditWidget);

    QVERIFY(!d.roleEditDialogInfo().isValid());
}

#include "moc_roleeditdialogtest.cpp"
