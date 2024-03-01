/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationloginwidgettest.h"
#include "common/authenticationloginwidget.h"
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QSignalSpy>
#include <QTest>
#include <QTestKeyEvent>
QTEST_MAIN(AuthenticationLoginWidgetTest)

AuthenticationLoginWidgetTest::AuthenticationLoginWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AuthenticationLoginWidgetTest::shouldHaveDefaultValues()
{
    AuthenticationLoginWidget w;

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAccountName = w.findChild<QLineEdit *>(QStringLiteral("mAccountName"));
    QVERIFY(mAccountName);
    QVERIFY(mAccountName->text().isEmpty());
    QVERIFY(mAccountName->isClearButtonEnabled());

    auto mServerUrl = w.findChild<QLineEdit *>(QStringLiteral("mServerUrl"));
    QVERIFY(mServerUrl);
    QVERIFY(mServerUrl->text().isEmpty());

    auto mUserName = w.findChild<QLineEdit *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());
    QVERIFY(mUserName->isClearButtonEnabled());
    QVERIFY(!mUserName->placeholderText().isEmpty());

    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QVERIFY(mPasswordLineEdit);
}

void AuthenticationLoginWidgetTest::shouldEmitSignalWhenPressEnter()
{
    AuthenticationLoginWidget w;
    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    mPasswordLineEdit->setPassword(QStringLiteral("ddd"));
    QSignalSpy spyTryLogin(&w, &AuthenticationLoginWidget::tryLogin);
    QVERIFY(mPasswordLineEdit);
    QTest::keyClick(mPasswordLineEdit->lineEdit(), Qt::Key_Enter);
    QCOMPARE(spyTryLogin.count(), 1);
    spyTryLogin.clear();
    QTest::keyClick(mPasswordLineEdit->lineEdit(), Qt::Key_O);
    QCOMPARE(spyTryLogin.count(), 0);
}

#include "moc_authenticationloginwidgettest.cpp"
