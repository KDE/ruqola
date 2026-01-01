/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaloginwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mRuqolaLoginStackWidget = w.findChild<RuqolaLoginStackWidget *>(u"mRuqolaLoginStackWidget"_s);
    QVERIFY(mRuqolaLoginStackWidget);

    auto mLoginButton = w.findChild<QPushButton *>(u"mLoginButton"_s);
    QVERIFY(mLoginButton);
    QVERIFY(!mLoginButton->text().isEmpty());

    auto mBusyIndicatorWidget = w.findChild<KBusyIndicatorWidget *>(u"mBusyIndicatorWidget"_s);
    QVERIFY(mBusyIndicatorWidget);

    auto mFailedError = w.findChild<QLabel *>(u"mFailedError"_s);
    QVERIFY(mFailedError);
    QVERIFY(mFailedError->text().isEmpty());
    QVERIFY(mFailedError->isHidden());
    QFont font = mFailedError->font();
    QVERIFY(font.bold());

    auto mTwoFactorAuthenticationWidget = w.findChild<QWidget *>(u"mTwoFactorAuthenticationWidget"_s);
    QVERIFY(mTwoFactorAuthenticationWidget);
    QVERIFY(!mTwoFactorAuthenticationWidget->isVisible());

    auto twoFactorAuthenticationLayout = w.findChild<QVBoxLayout *>(u"twoFactorAuthenticationLayout"_s);
    QVERIFY(twoFactorAuthenticationLayout);
    QCOMPARE(twoFactorAuthenticationLayout->contentsMargins(), QMargins{});

    auto twoFactorAuthenticationLabel = w.findChild<QLabel *>(u"twoFactorAuthenticationLabel"_s);
    QVERIFY(twoFactorAuthenticationLabel);
    QVERIFY(!twoFactorAuthenticationLabel->text().isEmpty());

    auto mTwoFactorAuthenticationPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mTwoFactorAuthenticationPasswordLineEdit"_s);
    QVERIFY(mTwoFactorAuthenticationPasswordLineEdit);

    auto sendNewEmailCode = w.findChild<QPushButton *>(u"sendNewEmailCode"_s);
    QVERIFY(sendNewEmailCode);
    QVERIFY(!sendNewEmailCode->text().isEmpty());
}

void RuqolaLoginWidgetTest::shouldShowLabelError()
{
    RuqolaLoginWidget w;
    auto mFailedError = w.findChild<QLabel *>(u"mFailedError"_s);
    QVERIFY(mFailedError);
    w.showError(u"bla"_s);

    QCOMPARE(mFailedError->text(), u"bla"_s);
    QVERIFY(!mFailedError->isHidden());
}

#include "moc_ruqolaloginwidgettest.cpp"
