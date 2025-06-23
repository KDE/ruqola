/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesscreatewidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto formLayout = w.findChild<QFormLayout *>(u"formLayout"_s);
    QVERIFY(formLayout);

    auto mBypassTwoFactor = w.findChild<QCheckBox *>(u"mBypassTwoFactor"_s);
    QVERIFY(mBypassTwoFactor);
    QVERIFY(!mBypassTwoFactor->isChecked());
    QVERIFY(!mBypassTwoFactor->text().isEmpty());

    auto mTokenName = w.findChild<QLineEdit *>(u"mTokenName"_s);
    QVERIFY(mTokenName);
    QVERIFY(mTokenName->text().isEmpty());
    QVERIFY(mTokenName->isClearButtonEnabled());
}

#include "moc_myaccountpersonalaccesscreatewidgettest.cpp"
