/*
   SPDX-FileCopyrightText: 2018-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "googlepluginauthenticationtest.h"
#include "../googlepluginauthentication.h"
#include <QTest>

QTEST_MAIN(GooglePluginAuthenticationTest)

GooglePluginAuthenticationTest::GooglePluginAuthenticationTest(QObject *parent)
    : QObject(parent)
{
}

void GooglePluginAuthenticationTest::shouldHaveDefaultValue()
{
    GooglePluginAuthentication w(nullptr, {});
    QVERIFY(!w.name().isEmpty());
    QCOMPARE(w.type(), AuthenticationManager::OauthType::Google);
}

#include "moc_googlepluginauthenticationtest.cpp"
