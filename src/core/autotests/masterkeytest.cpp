/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "masterkeytest.h"
#include "encryption/encryptionutils.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(MasterKeyTest)
MasterKeyTest::MasterKeyTest(QObject *parent)
    : QObject(parent)
{
}

/**
 * @brief This methods to going to test the determinism of the master key.
 *
 * n, n1 = salt
 *
 * m1, m2 = password
 *
 * if
 *
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
        masterKey1 = EncryptionUtils::getMasterKey(u"admin"_s, u"root"_s);
        masterKey2 = EncryptionUtils::getMasterKey(u"admin"_s, u"root"_s);
        QVERIFY(masterKey1 == masterKey2);

        masterKey1 = EncryptionUtils::getMasterKey(u"123"_s, u"abc"_s);
        masterKey2 = EncryptionUtils::getMasterKey(u"abc"_s, u"321"_s);
        QVERIFY(masterKey1 != masterKey2);
    }
}

void MasterKeyTest::masterKeyEmptyPasswordTest()
{
    const QByteArray masterKey = EncryptionUtils::getMasterKey(u""_s, u"someUser"_s);
    QVERIFY(masterKey.isEmpty());
}

void MasterKeyTest::masterKeyEmptyUserIdTest()
{
    const QByteArray masterKey = EncryptionUtils::getMasterKey(u"somePassword"_s, u""_s);
    QVERIFY(masterKey.isEmpty());
}

void MasterKeyTest::importRawKeyTest()
{
    QByteArray baseKey1;
    QByteArray baseKey2;

    for (int i = 0; i <= 10; i++) {
        baseKey1 = EncryptionUtils::importRawKey(u"admin"_s.toUtf8(), u"root"_s.toUtf8(), 1000);
        baseKey2 = EncryptionUtils::importRawKey(u"admin"_s.toUtf8(), u"root"_s.toUtf8(), 1000);
        QVERIFY(baseKey1 == baseKey2);

        baseKey1 = EncryptionUtils::importRawKey(u"admin"_s.toUtf8(), u"root"_s.toUtf8(), 1000);
        baseKey2 = EncryptionUtils::importRawKey(u"root"_s.toUtf8(), u"admin"_s.toUtf8(), 1000);
        QVERIFY(baseKey1 != baseKey2);
    }
}

#include "moc_masterkeytest.cpp"
