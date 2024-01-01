/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordauthenticationinterfacetest.h"
#include "../passwordauthenticationinterface.h"
#include <QTest>
QTEST_GUILESS_MAIN(PasswordAuthenticationInterfaceTest)

PasswordAuthenticationInterfaceTest::PasswordAuthenticationInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void PasswordAuthenticationInterfaceTest::shouldHaveDefaultValue()
{
    PasswordAuthenticationInterface w;
    QVERIFY(!w.account());
}

#include "moc_passwordauthenticationinterfacetest.cpp"
