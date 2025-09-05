/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "masterkeytest.h"
#include "encryption/encryptionutils.h"
#include <QTest>

QTEST_GUILESS_MAIN(MasterKeyTest)
MasterKeyTest::MasterKeyTest(QObject *parent)
    : QObject(parent)
{
}

/**
 * @brief This methods to going to test the determinism of the master key.
 *
 * if
 * n == n1 and m == m1
 * then
 * getMasterKey(n, m) == getMasterKey(n1, m1)
 *
 * if
 * n != n1 and m != m1
 * then
 * getMasterKey(n, m) != getMasterKey(n1, m1)
 */
void MasterKeyTest::masterKeyDeterminismTest()
{
    QByteArray masterKey1;
    QByteArray masterKey2;

    for (int i = 0; i <= 10; i++) {
        masterKey1 = EncryptionUtils::getMasterKey(QStringLiteral("admin"), QStringLiteral("root"));
        masterKey2 = EncryptionUtils::getMasterKey(QStringLiteral("admin"), QStringLiteral("root"));
        QVERIFY(masterKey1 == masterKey2);

        masterKey1 = EncryptionUtils::getMasterKey(QStringLiteral("123"), QStringLiteral("abc"));
        masterKey2 = EncryptionUtils::getMasterKey(QStringLiteral("abc"), QStringLiteral("321"));
        QVERIFY(masterKey1 != masterKey2);
    }
}

void MasterKeyTest::masterKeyEmptyPasswordTest()
{
    const QByteArray masterKey = EncryptionUtils::getMasterKey(QStringLiteral(""), QStringLiteral("someUser"));
    QVERIFY(masterKey.isEmpty());
}

void MasterKeyTest::masterKeyEmptyUserIdTest()
{
    const QByteArray masterKey = EncryptionUtils::getMasterKey(QStringLiteral("somePassword"), QStringLiteral(""));
    QVERIFY(masterKey.isEmpty());
}

void MasterKeyTest::importRawKeyTest()
{
    QByteArray baseKey1;
    QByteArray baseKey2;

    for (int i = 0; i <= 10; i++) {
        baseKey1 = EncryptionUtils::importRawKey(QStringLiteral("admin").toUtf8(), QStringLiteral("root").toUtf8(), 1000);
        baseKey2 = EncryptionUtils::importRawKey(QStringLiteral("admin").toUtf8(), QStringLiteral("root").toUtf8(), 1000);
        QVERIFY(baseKey1 == baseKey2);

        baseKey1 = EncryptionUtils::importRawKey(QStringLiteral("admin").toUtf8(), QStringLiteral("root").toUtf8(), 1000);
        baseKey2 = EncryptionUtils::importRawKey(QStringLiteral("root").toUtf8(), QStringLiteral("admin").toUtf8(), 1000);
        QVERIFY(baseKey1 != baseKey2);
    }
}

#include "moc_masterkeytest.cpp"
