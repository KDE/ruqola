/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "googleauthenticationinterfacetest.h"
#include "../googleauthenticationinterface.h"

#include <QTest>
QTEST_GUILESS_MAIN(GoogleAuthenticationInterfaceTest)

GoogleAuthenticationInterfaceTest::GoogleAuthenticationInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void GoogleAuthenticationInterfaceTest::shouldHaveDefaultValue()
{
    GoogleAuthenticationInterface w;
    QVERIFY(!w.account());
}

#include "moc_googleauthenticationinterfacetest.cpp"
