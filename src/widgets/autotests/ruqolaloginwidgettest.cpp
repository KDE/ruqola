/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaloginwidgettest.h"
#include "loginwidget/ruqolaloginstackwidget.h"
#include "loginwidget/ruqolaloginwidget.h"
#include <KBusyIndicatorWidget>
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(RuqolaLoginWidgetTest)
RuqolaLoginWidgetTest::RuqolaLoginWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void RuqolaLoginWidgetTest::shouldHaveDefaultValues()
{
    RuqolaLoginWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mRuqolaLoginStackWidget = w.findChild<RuqolaLoginStackWidget *>(QStringLiteral("mRuqolaLoginStackWidget"));
    QVERIFY(mRuqolaLoginStackWidget);

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
