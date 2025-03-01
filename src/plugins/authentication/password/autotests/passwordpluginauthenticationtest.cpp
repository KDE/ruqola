/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordpluginauthenticationtest.h"
#include "../passwordpluginauthentication.h"
#include <QTest>

QTEST_MAIN(PasswordPluginAuthenticationTest)

PasswordPluginAuthenticationTest::PasswordPluginAuthenticationTest(QObject *parent)
    : QObject(parent)
{
}

void PasswordPluginAuthenticationTest::shouldHaveDefaultValue()
{
    PasswordPluginAuthentication w(nullptr, {});
    QVERIFY(!w.name().isEmpty());
    QCOMPARE(w.authenticationType(), AuthenticationManager::AuthMethodType::Password);
}

#include "moc_passwordpluginauthenticationtest.cpp"
