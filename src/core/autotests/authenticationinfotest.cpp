/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationinfotest.h"
#include "authenticationinfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(AuthenticationInfoTest)

AuthenticationInfoTest::AuthenticationInfoTest(QObject *parent)
    : QObject(parent)
{
}

void AuthenticationInfoTest::shouldHaveDefaultValue()
{
    AuthenticationInfo info;
    QVERIFY(info.iconName().isEmpty());
    QVERIFY(info.name().isEmpty());
    QCOMPARE(info.oauthType(), AuthenticationManager::Unknown);
}

void AuthenticationInfoTest::shouldAssignValue()
{
    AuthenticationInfo info;
    const QString name = QStringLiteral("name1");
    const QString iconname = QStringLiteral("iconname1");
    info.setName(name);
    info.setIconName(iconname);
    QCOMPARE(info.name(), name);
    QCOMPARE(info.iconName(), iconname);
    QCOMPARE(info.oauthType(), AuthenticationManager::Unknown);
}

void AuthenticationInfoTest::shouldBeValid()
{
    AuthenticationInfo info;
    QVERIFY(!info.isValid());

    const QString name = QStringLiteral("name1");
    const QString iconname = QStringLiteral("iconname1");
    info.setName(name);
    QVERIFY(!info.isValid());
    info.setIconName(iconname);
    QVERIFY(!info.isValid());

    info.setOauthType(AuthenticationManager::Twitter);
    QVERIFY(info.isValid());

    info.setIconName(QString());
    QVERIFY(info.isValid());

    info.setName(QString());
    QVERIFY(!info.isValid());
}

#include "moc_authenticationinfotest.cpp"
