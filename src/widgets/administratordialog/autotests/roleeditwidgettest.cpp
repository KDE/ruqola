/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roleeditwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/roles/roleeditwidget.h"
#include "administratordialog/roles/rolescopecombobox.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(RoleEditWidgetTest)
RoleEditWidgetTest::RoleEditWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RoleEditWidgetTest::shouldHaveDefaultValues()
{
    RoleEditWidget w;
    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mName = w.findChild<QLineEdit *>(u"mName"_s);
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());

    auto mDescription = w.findChild<QLineEdit *>(u"mDescription"_s);
    QVERIFY(mDescription);
    QVERIFY(mDescription->text().isEmpty());

    auto mTwoFactor = w.findChild<QCheckBox *>(u"mTwoFactor"_s);
    QVERIFY(mTwoFactor);
    QVERIFY(!mTwoFactor->isChecked());
    QVERIFY(!mTwoFactor->text().isEmpty());

    auto mRoleScopeComboBox = w.findChild<RoleScopeComboBox *>(u"mRoleScopeComboBox"_s);
    QVERIFY(mRoleScopeComboBox);

    QVERIFY(!w.roleEditDialogInfo().isValid());
}

void RoleEditWidgetTest::shouldVerifyRoleEditDialogInfoValues()
{
    RoleEditWidget::RoleEditDialogInfo info;
    QVERIFY(!info.isValid());
    QVERIFY(info.mName.isEmpty());
    QVERIFY(info.mDescription.isEmpty());
    QVERIFY(info.mScope.isEmpty());
    QVERIFY(!info.mTwoFactor);
    QVERIFY(!info.mIsProtected);
}

#include "moc_roleeditwidgettest.cpp"
