/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserdialogtest.h"

#include "registeruser/registeruserdialog.h"
#include "registeruser/registeruserwidget.h"

#include <KPasswordLineEdit>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSignalSpy>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(RegisterUserDialogTest)

using namespace Qt::Literals::StringLiterals;

namespace
{
void fillRegisterUserForm(const RegisterUserDialog &w, const QString &userName, const QString &email, const QString &password)
{
    auto mUserName = w.findChild<QLineEdit *>(u"mUserName"_s);
    QVERIFY(mUserName);
    mUserName->setText(userName);

    auto mEmail = w.findChild<QLineEdit *>(u"mEmail"_s);
    QVERIFY(mEmail);
    mEmail->setText(email);

    auto mNewPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mNewPasswordLineEdit"_s);
    QVERIFY(mNewPasswordLineEdit);
    mNewPasswordLineEdit->setPassword(password);

    auto mConfirmPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mConfirmPasswordLineEdit"_s);
    QVERIFY(mConfirmPasswordLineEdit);
    mConfirmPasswordLineEdit->setPassword(password);
}
}

RegisterUserDialogTest::RegisterUserDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void RegisterUserDialogTest::shouldHaveDefaultValues()
{
    const RegisterUserDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mRegisterUserWidget = w.findChild<RegisterUserWidget *>(u"mRegisterUserWidget"_s);
    QVERIFY(mRegisterUserWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

void RegisterUserDialogTest::shouldEmitRegisterNewAccount()
{
    const RegisterUserDialog w(nullptr);

    auto mRegisterButton = w.findChild<QPushButton *>(u"mRegisterButton"_s);
    QVERIFY(mRegisterButton);
    QVERIFY(!mRegisterButton->isEnabled());

    const QSignalSpy registerNewAccountSpy(&w, &RegisterUserDialog::registerNewAccount);

    fillRegisterUserForm(w, u"foo"_s, u"foo@kde.org"_s, u"foopassword"_s);

    QVERIFY(mRegisterButton->isEnabled());
    QCOMPARE(registerNewAccountSpy.count(), 0);

    // The widget signal must be forwarded by the dialog
    mRegisterButton->click();
    QCOMPARE(registerNewAccountSpy.count(), 1);
    QVERIFY(!mRegisterButton->isEnabled());
}

void RegisterUserDialogTest::shouldReturnRegisterUserInfo()
{
    const RegisterUserDialog w(nullptr);
    fillRegisterUserForm(w, u"  foo bar  "_s, u"  foo@kde.org  "_s, u"foopassword"_s);

    const RocketChatRestApi::RegisterUserJob::RegisterUserInfo info = w.registerUserInfo();
    QCOMPARE(info.name, u"foo bar"_s);
    QCOMPARE(info.username, u"foobar"_s);
    QCOMPARE(info.email, u"foo@kde.org"_s);
    QCOMPARE(info.password, u"foopassword"_s);
    // Reason is not asked for by default
    QVERIFY(info.reason.isEmpty());
    QVERIFY(info.isValid());
}

void RegisterUserDialogTest::shouldReturnReasonWhenManuallyApproveNewUsersIsRequired()
{
    RegisterUserDialog w(nullptr);
    w.setManuallyApproveNewUsersRequired(true);
    fillRegisterUserForm(w, u"foo"_s, u"foo@kde.org"_s, u"foopassword"_s);

    auto mRegisterButton = w.findChild<QPushButton *>(u"mRegisterButton"_s);
    QVERIFY(mRegisterButton);
    // A reason is mandatory now
    QVERIFY(!mRegisterButton->isEnabled());

    auto mReasonTextEdit = w.findChild<QPlainTextEdit *>(u"mReasonTextEdit"_s);
    QVERIFY(mReasonTextEdit);
    mReasonTextEdit->setPlainText(u"a reason"_s);
    QVERIFY(mRegisterButton->isEnabled());

    QCOMPARE(w.registerUserInfo().reason, u"a reason"_s);
}

#include "moc_registeruserdialogtest.cpp"
