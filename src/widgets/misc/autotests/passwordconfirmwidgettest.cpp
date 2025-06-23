/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordconfirmwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/passwordconfirmwidget.h"
#include "misc/passwordvalidatewidget.h"
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QTest>
QTEST_MAIN(PasswordConfirmWidgetTest)
PasswordConfirmWidgetTest::PasswordConfirmWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void PasswordConfirmWidgetTest::shouldHaveDefaultValues()
{
    PasswordConfirmWidget w;

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mNewPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mNewPasswordLineEdit"_s);
    QVERIFY(mNewPasswordLineEdit);

    auto mConfirmPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mConfirmPasswordLineEdit"_s);
    QVERIFY(mConfirmPasswordLineEdit);

    QVERIFY(!w.isNewPasswordConfirmed());

    auto mPasswordValidateWidget = w.findChild<PasswordValidateWidget *>(u"mPasswordValidateWidget"_s);
    QVERIFY(mPasswordValidateWidget);
}

void PasswordConfirmWidgetTest::shouldReturnNewPassword()
{
    PasswordConfirmWidget w;
    auto mNewPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mNewPasswordLineEdit"_s);

    auto mConfirmPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mConfirmPasswordLineEdit"_s);
    QVERIFY(!w.isNewPasswordConfirmed());
    mNewPasswordLineEdit->setPassword(u"bla"_s);
    // mConfirmPasswordLineEdit is Empty => not a new password
    QVERIFY(!w.isNewPasswordConfirmed());

    mConfirmPasswordLineEdit->setPassword(u"bli"_s);
    QVERIFY(!w.isNewPasswordConfirmed());

    mConfirmPasswordLineEdit->setPassword(u"bla"_s);
    QVERIFY(w.isNewPasswordConfirmed());
}

#include "moc_passwordconfirmwidgettest.cpp"
