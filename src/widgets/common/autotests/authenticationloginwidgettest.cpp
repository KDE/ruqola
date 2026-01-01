/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationloginwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "common/authenticationloginwidget.h"
#include <KPasswordLineEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(AuthenticationLoginWidgetTest)

AuthenticationLoginWidgetTest::AuthenticationLoginWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AuthenticationLoginWidgetTest::shouldHaveDefaultValues()
{
    AuthenticationLoginWidget w;

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAccountName = w.findChild<QLineEdit *>(u"mAccountName"_s);
    QVERIFY(mAccountName);
    QVERIFY(mAccountName->text().isEmpty());
    QVERIFY(mAccountName->isClearButtonEnabled());

    auto mServerUrl = w.findChild<QLineEdit *>(u"mServerUrl"_s);
    QVERIFY(mServerUrl);
    QVERIFY(mServerUrl->text().isEmpty());

    auto mUserName = w.findChild<QLineEdit *>(u"mUserName"_s);
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());
    QVERIFY(mUserName->isClearButtonEnabled());
    QVERIFY(!mUserName->placeholderText().isEmpty());

    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mPasswordLineEdit"_s);
    QVERIFY(mPasswordLineEdit);
}

void AuthenticationLoginWidgetTest::shouldEmitSignalWhenPressEnter()
{
    AuthenticationLoginWidget w;
    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(u"mPasswordLineEdit"_s);
    mPasswordLineEdit->setPassword(u"ddd"_s);
    QSignalSpy spyTryLogin(&w, &AuthenticationLoginWidget::tryLogin);
    QVERIFY(mPasswordLineEdit);
    QTest::keyClick(mPasswordLineEdit->lineEdit(), Qt::Key_Enter);
    QCOMPARE(spyTryLogin.count(), 1);
    spyTryLogin.clear();
    QTest::keyClick(mPasswordLineEdit->lineEdit(), Qt::Key_O);
    QCOMPARE(spyTryLogin.count(), 0);
}

#include "moc_authenticationloginwidgettest.cpp"
