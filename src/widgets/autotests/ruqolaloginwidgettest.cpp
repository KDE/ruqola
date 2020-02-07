/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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
#include "ruqolaloginwidget.h"
#include <KBusyIndicatorWidget>
#include <KPasswordLineEdit>
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

    auto *mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto *mAccountName = w.findChild<QLabel *>(QStringLiteral("mAccountName"));
    QVERIFY(mAccountName);
    QVERIFY(mAccountName->text().isEmpty());

    auto *mServerName = w.findChild<QLineEdit *>(QStringLiteral("mServerName"));
    QVERIFY(mServerName);
    QVERIFY(mServerName->text().isEmpty());

    auto *mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());

    auto *mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QVERIFY(mPasswordLineEdit);

    auto *mLoginButton = w.findChild<QPushButton *>(QStringLiteral("mLoginButton"));
    QVERIFY(mLoginButton);
    QVERIFY(!mLoginButton->text().isEmpty());

    auto *mBusyIndicatorWidget = w.findChild<KBusyIndicatorWidget *>(QStringLiteral("mBusyIndicatorWidget"));
    QVERIFY(mBusyIndicatorWidget);

    auto *mFailedError = w.findChild<QLabel *>(QStringLiteral("mFailedError"));
    QVERIFY(mFailedError);
    QVERIFY(mFailedError->text().isEmpty());
    QVERIFY(mFailedError->isHidden());


    QWidget *mAuthenticationWidget = w.findChild<QWidget *>(QStringLiteral("authenticationWidget"));
    QVERIFY(mAuthenticationWidget);
    QVERIFY(!mAuthenticationWidget->isVisible());

    QVBoxLayout *twoFactorAuthenticationLayout = w.findChild<QVBoxLayout *>(QStringLiteral("twoFactorAuthenticationLayout"));
    QVERIFY(twoFactorAuthenticationLayout);

    QLabel *mTwoFactorAuthenticationLabel = w.findChild<QLabel *>(QStringLiteral("mTwoFactorAuthenticationLabel"));
    QVERIFY(mTwoFactorAuthenticationLabel);
    QVERIFY(!mTwoFactorAuthenticationLabel->text().isEmpty());

    KPasswordLineEdit *mTwoFactorAuthenticationPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mTwoFactorAuthenticationPasswordLineEdit"));
    QVERIFY(mTwoFactorAuthenticationPasswordLineEdit);
}
