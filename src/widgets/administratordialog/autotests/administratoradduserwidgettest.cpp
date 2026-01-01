/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoradduserwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/users/administratoradduserwidget.h"
#include "misc/rolescombobox.h"
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(AdministratorAddUserWidgetTest)
AdministratorAddUserWidgetTest::AdministratorAddUserWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorAddUserWidgetTest::shouldHaveDefaultValues()
{
    AdministratorAddUserWidget w(nullptr);
    auto formLayout = w.findChild<QFormLayout *>(u"formLayout"_s);
    QVERIFY(formLayout);
    auto mName = w.findChild<QLineEdit *>(u"mName"_s);
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());

    auto mUserName = w.findChild<QLineEdit *>(u"mUserName"_s);
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());

    auto mEmail = w.findChild<QLineEdit *>(u"mEmail"_s);
    QVERIFY(mEmail);
    QVERIFY(mEmail->text().isEmpty());

    auto mStatusText = w.findChild<QLineEdit *>(u"mStatusText"_s);
    QVERIFY(mStatusText);
    QVERIFY(mStatusText->text().isEmpty());

    auto mNickName = w.findChild<QLineEdit *>(u"mNickName"_s);
    QVERIFY(mNickName);
    QVERIFY(mNickName->text().isEmpty());

    auto mSendWelcomeEmails = w.findChild<QCheckBox *>(u"mSendWelcomeEmails"_s);
    QVERIFY(mSendWelcomeEmails);
    QVERIFY(!mSendWelcomeEmails->text().isEmpty());
    QVERIFY(!mSendWelcomeEmails->isChecked());

    auto mRequirePassword = w.findChild<QCheckBox *>(u"mRequirePassword"_s);
    QVERIFY(mRequirePassword);
    QVERIFY(!mRequirePassword->text().isEmpty());
    QVERIFY(!mRequirePassword->isChecked());

    auto mSetRandowPassword = w.findChild<QCheckBox *>(u"mSetRandowPassword"_s);
    QVERIFY(mSetRandowPassword);
    QVERIFY(!mSetRandowPassword->text().isEmpty());
    QVERIFY(!mSetRandowPassword->isChecked());

    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mPasswordLineEdit"_s);
    QVERIFY(mPasswordLineEdit);
    QVERIFY(mPasswordLineEdit->password().isEmpty());

    auto mRolesComboBox = w.findChild<RolesComboBox *>(u"mRolesComboBox"_s);
    QVERIFY(mRolesComboBox);

    auto mEmailVerified = w.findChild<QCheckBox *>(u"mEmailVerified"_s);
    QVERIFY(mEmailVerified);
    QVERIFY(!mEmailVerified->text().isEmpty());
    QVERIFY(!mEmailVerified->isChecked());
}

#include "moc_administratoradduserwidgettest.cpp"
