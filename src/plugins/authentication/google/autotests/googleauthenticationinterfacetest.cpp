/*
   SPDX-FileCopyrightText: 2018-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
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
