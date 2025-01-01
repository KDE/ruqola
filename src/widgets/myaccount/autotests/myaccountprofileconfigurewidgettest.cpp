/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountprofileconfigurewidgettest.h"
#include "misc/passwordconfirmwidget.h"
#include "myaccount/myaccountprofileconfigureavatarwidget.h"
#include "myaccount/myaccountprofileconfigurewidget.h"
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTest>
QTEST_MAIN(MyAccountProfileConfigureWidgetTest)

MyAccountProfileConfigureWidgetTest::MyAccountProfileConfigureWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void MyAccountProfileConfigureWidgetTest::shouldHaveDefaultValues()
{
    MyAccountProfileConfigureWidget w(nullptr);

    auto topLayout = w.findChild<QVBoxLayout *>(QStringLiteral("topLayout"));
    QVERIFY(topLayout);

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);

    auto mUserNameInfo = w.findChild<QLabel *>(QStringLiteral("mUserNameInfo"));
    QVERIFY(mUserNameInfo);
    QVERIFY(!mUserNameInfo->text().isEmpty());

    auto mEmail = w.findChild<QLineEdit *>(QStringLiteral("mEmail"));
    QVERIFY(mEmail);

    auto mEmailInfo = w.findChild<QLabel *>(QStringLiteral("mEmailInfo"));
    QVERIFY(mEmailInfo);
    QVERIFY(!mEmailInfo->text().isEmpty());

    auto mName = w.findChild<QLineEdit *>(QStringLiteral("mName"));
    QVERIFY(mName);
    QVERIFY(mName->isClearButtonEnabled());

    auto mNickName = w.findChild<QLineEdit *>(QStringLiteral("mNickName"));
    QVERIFY(mNickName);
    QVERIFY(mNickName->isClearButtonEnabled());

    auto mStatusText = w.findChild<QLineEdit *>(QStringLiteral("mStatusText"));
    QVERIFY(mStatusText);
    QVERIFY(mStatusText->isClearButtonEnabled());

    auto mStatusTextInfo = w.findChild<QLabel *>(QStringLiteral("mStatusTextInfo"));
    QVERIFY(mStatusTextInfo);
    QVERIFY(!mStatusTextInfo->text().isEmpty());

    auto mDeleteMyAccount = w.findChild<QPushButton *>(QStringLiteral("mDeleteMyAccount"));
    QVERIFY(mDeleteMyAccount);
    QVERIFY(!mDeleteMyAccount->text().isEmpty());

    auto mPasswordConfirmWidget = w.findChild<PasswordConfirmWidget *>(QStringLiteral("mPasswordConfirmWidget"));
    QVERIFY(mPasswordConfirmWidget);

    auto mLogoutFromOtherLocation = w.findChild<QPushButton *>(QStringLiteral("mLogoutFromOtherLocation"));
    QVERIFY(mLogoutFromOtherLocation);
    QVERIFY(!mLogoutFromOtherLocation->text().isEmpty());

    auto mConfigureAvatarWidget = w.findChild<MyAccountProfileConfigureAvatarWidget *>(QStringLiteral("mConfigureAvatarWidget"));
    QVERIFY(mConfigureAvatarWidget);
}

#include "moc_myaccountprofileconfigurewidgettest.cpp"
