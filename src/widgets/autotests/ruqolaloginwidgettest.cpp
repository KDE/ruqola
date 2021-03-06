/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

    auto mServerName = w.findChild<QLineEdit *>(QStringLiteral("mServerName"));
    QVERIFY(mServerName);
    QVERIFY(mServerName->text().isEmpty());

    auto mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());

    auto mPasswordLineEditWidget = w.findChild<PasswordLineEditWidget *>(QStringLiteral("mPasswordLineEditWidget"));
    QVERIFY(mPasswordLineEditWidget);

    auto mLdapCheckBox = w.findChild<QCheckBox *>(QStringLiteral("mLdapCheckBox"));
    QVERIFY(mLdapCheckBox);
    QVERIFY(!mLdapCheckBox->isChecked());

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

    auto mAuthenticationWidget = w.findChild<QWidget *>(QStringLiteral("authenticationWidget"));
    QVERIFY(mAuthenticationWidget);
    QVERIFY(!mAuthenticationWidget->isVisible());

    auto twoFactorAuthenticationLayout = w.findChild<QVBoxLayout *>(QStringLiteral("twoFactorAuthenticationLayout"));
    QVERIFY(twoFactorAuthenticationLayout);

    auto twoFactorAuthenticationLabel = w.findChild<QLabel *>(QStringLiteral("twoFactorAuthenticationLabel"));
    QVERIFY(twoFactorAuthenticationLabel);
    QVERIFY(!twoFactorAuthenticationLabel->text().isEmpty());

    auto mTwoFactorAuthenticationPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mTwoFactorAuthenticationPasswordLineEdit"));
    QVERIFY(mTwoFactorAuthenticationPasswordLineEdit);

    auto sendNewEmailCode = w.findChild<QPushButton *>(QStringLiteral("sendNewEmailCode"));
    QVERIFY(sendNewEmailCode);
    QVERIFY(!sendNewEmailCode->text().isEmpty());
}
