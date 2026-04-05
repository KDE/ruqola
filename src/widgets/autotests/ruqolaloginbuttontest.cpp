/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaloginbuttontest.h"
#include "loginwidget/ruqolaloginbutton.h"
#include <QTest>
QTEST_MAIN(RuqolaLoginButtonTest)

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

#include "moc_ruqolaloginbuttontest.cpp"
