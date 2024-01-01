/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaloginwidgettest.h"
#include "misc/passwordlineeditwidget.h"
#include "ruqolaloginwidget.h"
#include <KBusyIndicatorWidget>
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTest>

QTEST_MAIN(RuqolaLoginWidgetTest)
RuqolaLoginWidgetTest::RuqolaLoginWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void RuqolaLoginWidgetTest::shouldHaveDefaultValues()
{
    RuqolaLoginWidget w;

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mAccountName = w.findChild<QLineEdit *>(QStringLiteral("mAccountName"));
    QVERIFY(mAccountName);
    QVERIFY(mAccountName->text().isEmpty());

    auto mServerUrl = w.findChild<QLineEdit *>(QStringLiteral("mServerUrl"));
    QVERIFY(mServerUrl);
    QVERIFY(mServerUrl->text().isEmpty());

    auto mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());

    auto mPasswordLineEditWidget = w.findChild<PasswordLineEditWidget *>(QStringLiteral("mPasswordLineEditWidget"));
    QVERIFY(mPasswordLineEditWidget);

    //    auto mLdapCheckBox = w.findChild<QCheckBox *>(QStringLiteral("mLdapCheckBox"));
    //    QVERIFY(mLdapCheckBox);
    //    QVERIFY(!mLdapCheckBox->isChecked());

    auto mLoginButton = w.findChild<QPushButton *>(QStringLiteral("mLoginButton"));
    QVERIFY(mLoginButton);
    QVERIFY(!mLoginButton->text().isEmpty());

    auto mBusyIndicatorWidget = w.findChild<KBusyIndicatorWidget *>(QStringLiteral("mBusyIndicatorWidget"));
    QVERIFY(mBusyIndicatorWidget);

    auto mFailedError = w.findChild<QLabel *>(QStringLiteral("mFailedError"));
    QVERIFY(mFailedError);
    QVERIFY(mFailedError->text().isEmpty());
    QVERIFY(mFailedError->isHidden());
    QFont font = mFailedError->font();
    QVERIFY(font.bold());

    auto mTwoFactorAuthenticationWidget = w.findChild<QWidget *>(QStringLiteral("mTwoFactorAuthenticationWidget"));
    QVERIFY(mTwoFactorAuthenticationWidget);
    QVERIFY(!mTwoFactorAuthenticationWidget->isVisible());

    auto twoFactorAuthenticationLayout = w.findChild<QVBoxLayout *>(QStringLiteral("twoFactorAuthenticationLayout"));
    QVERIFY(twoFactorAuthenticationLayout);
    QCOMPARE(twoFactorAuthenticationLayout->contentsMargins(), QMargins{});

    auto twoFactorAuthenticationLabel = w.findChild<QLabel *>(QStringLiteral("twoFactorAuthenticationLabel"));
    QVERIFY(twoFactorAuthenticationLabel);
    QVERIFY(!twoFactorAuthenticationLabel->text().isEmpty());

    auto mTwoFactorAuthenticationPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mTwoFactorAuthenticationPasswordLineEdit"));
    QVERIFY(mTwoFactorAuthenticationPasswordLineEdit);

    auto sendNewEmailCode = w.findChild<QPushButton *>(QStringLiteral("sendNewEmailCode"));
    QVERIFY(sendNewEmailCode);
    QVERIFY(!sendNewEmailCode->text().isEmpty());
}

void RuqolaLoginWidgetTest::shouldShowLabelError()
{
    RuqolaLoginWidget w;
    auto mFailedError = w.findChild<QLabel *>(QStringLiteral("mFailedError"));
    QVERIFY(mFailedError);
    w.showError(QStringLiteral("bla"));

    QCOMPARE(mFailedError->text(), QStringLiteral("bla"));
    QVERIFY(!mFailedError->isHidden());
}

#include "moc_ruqolaloginwidgettest.cpp"
