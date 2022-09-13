/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesscreatewidgettest.h"
#include "myaccount/myaccountpersonalaccesscreatewidget.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>

QTEST_MAIN(MyAccountPersonalAccessCreateWidgetTest)
MyAccountPersonalAccessCreateWidgetTest::MyAccountPersonalAccessCreateWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MyAccountPersonalAccessCreateWidgetTest::shouldHaveDefaultValues()
{
    MyAccountPersonalAccessCreateWidget w;
    auto formLayout = w.findChild<QFormLayout *>(QStringLiteral("formLayout"));
    QVERIFY(formLayout);

    auto mBypassTwoFactor = w.findChild<QCheckBox *>(QStringLiteral("mBypassTwoFactor"));
    QVERIFY(mBypassTwoFactor);
    QVERIFY(!mBypassTwoFactor->isChecked());
    QVERIFY(!mBypassTwoFactor->text().isEmpty());

    auto mTokenName = w.findChild<QLineEdit *>(QStringLiteral("mTokenName"));
    QVERIFY(mTokenName);
    QVERIFY(mTokenName->text().isEmpty());
    QVERIFY(mTokenName->isClearButtonEnabled());
}
