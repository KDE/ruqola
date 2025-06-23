/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountprofileconfigurewidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto topLayout = w.findChild<QVBoxLayout *>(u"topLayout"_s);
    QVERIFY(topLayout);

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mUserName = w.findChild<QLineEdit *>(u"mUserName"_s);
    QVERIFY(mUserName);

    auto mUserNameInfo = w.findChild<QLabel *>(u"mUserNameInfo"_s);
    QVERIFY(mUserNameInfo);
    QVERIFY(!mUserNameInfo->text().isEmpty());

    auto mEmail = w.findChild<QLineEdit *>(u"mEmail"_s);
    QVERIFY(mEmail);

    auto mEmailInfo = w.findChild<QLabel *>(u"mEmailInfo"_s);
    QVERIFY(mEmailInfo);
    QVERIFY(!mEmailInfo->text().isEmpty());

    auto mName = w.findChild<QLineEdit *>(u"mName"_s);
    QVERIFY(mName);
    QVERIFY(mName->isClearButtonEnabled());

    auto mNickName = w.findChild<QLineEdit *>(u"mNickName"_s);
    QVERIFY(mNickName);
    QVERIFY(mNickName->isClearButtonEnabled());

    auto mStatusText = w.findChild<QLineEdit *>(u"mStatusText"_s);
    QVERIFY(mStatusText);
    QVERIFY(mStatusText->isClearButtonEnabled());

    auto mStatusTextInfo = w.findChild<QLabel *>(u"mStatusTextInfo"_s);
    QVERIFY(mStatusTextInfo);
    QVERIFY(!mStatusTextInfo->text().isEmpty());

    auto mDeleteMyAccount = w.findChild<QPushButton *>(u"mDeleteMyAccount"_s);
    QVERIFY(mDeleteMyAccount);
    QVERIFY(!mDeleteMyAccount->text().isEmpty());

    auto mPasswordConfirmWidget = w.findChild<PasswordConfirmWidget *>(u"mPasswordConfirmWidget"_s);
    QVERIFY(mPasswordConfirmWidget);

    auto mLogoutFromOtherLocation = w.findChild<QPushButton *>(u"mLogoutFromOtherLocation"_s);
    QVERIFY(mLogoutFromOtherLocation);
    QVERIFY(!mLogoutFromOtherLocation->text().isEmpty());

    auto mConfigureAvatarWidget = w.findChild<MyAccountProfileConfigureAvatarWidget *>(u"mConfigureAvatarWidget"_s);
    QVERIFY(mConfigureAvatarWidget);
}

#include "moc_myaccountprofileconfigurewidgettest.cpp"
