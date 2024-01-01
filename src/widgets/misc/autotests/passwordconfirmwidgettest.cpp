/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordconfirmwidgettest.h"
#include "misc/passwordconfirmwidget.h"
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

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mNewPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mNewPasswordLineEdit"));
    QVERIFY(mNewPasswordLineEdit);

    auto mConfirmPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mConfirmPasswordLineEdit"));
    QVERIFY(mConfirmPasswordLineEdit);

    QVERIFY(!w.isNewPasswordConfirmed());
}

void PasswordConfirmWidgetTest::shouldReturnNewPassword()
{
    PasswordConfirmWidget w;
    auto mNewPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mNewPasswordLineEdit"));

    auto mConfirmPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mConfirmPasswordLineEdit"));
    QVERIFY(!w.isNewPasswordConfirmed());
    mNewPasswordLineEdit->setPassword(QStringLiteral("bla"));
    // mConfirmPasswordLineEdit is Empty => not a new password
    QVERIFY(!w.isNewPasswordConfirmed());

    mConfirmPasswordLineEdit->setPassword(QStringLiteral("bli"));
    QVERIFY(!w.isNewPasswordConfirmed());

    mConfirmPasswordLineEdit->setPassword(QStringLiteral("bla"));
    QVERIFY(w.isNewPasswordConfirmed());
}

#include "moc_passwordconfirmwidgettest.cpp"
