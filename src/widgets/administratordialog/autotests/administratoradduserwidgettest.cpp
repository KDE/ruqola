/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoradduserwidgettest.h"
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
    auto formLayout = w.findChild<QFormLayout *>(QStringLiteral("formLayout"));
    QVERIFY(formLayout);
    auto mName = w.findChild<QLineEdit *>(QStringLiteral("mName"));
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());

    auto mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());

    auto mEmail = w.findChild<QLineEdit *>(QStringLiteral("mEmail"));
    QVERIFY(mEmail);
    QVERIFY(mEmail->text().isEmpty());

    auto mStatusText = w.findChild<QLineEdit *>(QStringLiteral("mStatusText"));
    QVERIFY(mStatusText);
    QVERIFY(mStatusText->text().isEmpty());

    auto mNickName = w.findChild<QLineEdit *>(QStringLiteral("mNickName"));
    QVERIFY(mNickName);
    QVERIFY(mNickName->text().isEmpty());

    auto mJoinDefaultChannels = w.findChild<QCheckBox *>(QStringLiteral("mJoinDefaultChannels"));
    QVERIFY(mJoinDefaultChannels);
    QVERIFY(!mJoinDefaultChannels->text().isEmpty());
    QVERIFY(!mJoinDefaultChannels->isChecked());

    auto mSendWelcomeEmails = w.findChild<QCheckBox *>(QStringLiteral("mSendWelcomeEmails"));
    QVERIFY(mSendWelcomeEmails);
    QVERIFY(!mSendWelcomeEmails->text().isEmpty());
    QVERIFY(!mSendWelcomeEmails->isChecked());

    auto mRequirePassword = w.findChild<QCheckBox *>(QStringLiteral("mRequirePassword"));
    QVERIFY(mRequirePassword);
    QVERIFY(!mRequirePassword->text().isEmpty());
    QVERIFY(!mRequirePassword->isChecked());

    auto mSetRandowPassword = w.findChild<QCheckBox *>(QStringLiteral("mSetRandowPassword"));
    QVERIFY(mSetRandowPassword);
    QVERIFY(!mSetRandowPassword->text().isEmpty());
    QVERIFY(!mSetRandowPassword->isChecked());

    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QVERIFY(mPasswordLineEdit);
    QVERIFY(mPasswordLineEdit->password().isEmpty());

    auto mRolesComboBox = w.findChild<RolesComboBox *>(QStringLiteral("mRolesComboBox"));
    QVERIFY(mRolesComboBox);

    auto mEmailVerified = w.findChild<QCheckBox *>(QStringLiteral("mEmailVerified"));
    QVERIFY(mEmailVerified);
    QVERIFY(!mEmailVerified->text().isEmpty());
    QVERIFY(!mEmailVerified->isChecked());
}

#include "moc_administratoradduserwidgettest.cpp"
