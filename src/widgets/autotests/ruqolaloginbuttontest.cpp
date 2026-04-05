/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaloginbuttontest.h"
#include "loginwidget/ruqolaloginbutton.h"
#include <QSignalSpy>
#include <QTest>
#include <qtestmouse.h>
QTEST_MAIN(RuqolaLoginButtonTest)
using namespace Qt::Literals::StringLiterals;

RuqolaLoginButtonTest::RuqolaLoginButtonTest(QObject *parent)
    : QObject{parent}
{
}

void RuqolaLoginButtonTest::shouldHaveDefaultValues()
{
    const RuqolaLoginButton b;
    QVERIFY(!b.loginInProgress());
    QVERIFY(!b.text().isEmpty());
}

void RuqolaLoginButtonTest::shouldEmitSignal()
{
    RuqolaLoginButton b;
    QSignalSpy spyLoginRequested(&b, &RuqolaLoginButton::loginRequested);
    QSignalSpy spyCancelLoginRequested(&b, &RuqolaLoginButton::cancelLoginRequested);

    QVERIFY(!b.loginInProgress());
    QTest::mouseClick(&b, Qt::LeftButton);
    QCOMPARE(spyLoginRequested.count(), 1);
    QCOMPARE(spyCancelLoginRequested.count(), 0);

    spyLoginRequested.clear();
    spyCancelLoginRequested.clear();

    b.setLoginInProgress(true);
    QVERIFY(b.loginInProgress());
    QTest::mouseClick(&b, Qt::LeftButton);
    QCOMPARE(spyLoginRequested.count(), 0);
    QCOMPARE(spyCancelLoginRequested.count(), 1);

    spyLoginRequested.clear();
    spyCancelLoginRequested.clear();

    b.setLoginInProgress(false);
    QVERIFY(!b.loginInProgress());
    QTest::mouseClick(&b, Qt::LeftButton);
    QCOMPARE(spyLoginRequested.count(), 1);
    QCOMPARE(spyCancelLoginRequested.count(), 0);
}

#include "moc_ruqolaloginbuttontest.cpp"
