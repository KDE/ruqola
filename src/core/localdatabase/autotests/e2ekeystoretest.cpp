/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "e2ekeystoretest.h"
#include "encryption/encryptionutils.h"
#include "localdatabase/e2ekeystore.h"
#include <QtTest>

QTEST_GUILESS_MAIN(E2EKeyStoreTest)

const auto testUser = QStringLiteral("testuser");
const auto otherUser = QStringLiteral("otheruser");

void E2EKeyStoreTest::initTestCase()
{
    E2EKeyStore store;
    store.deleteKey(testUser);
    store.deleteKey(otherUser);
}

void E2EKeyStoreTest::testSaveLoadDelete()
{
    E2EKeyStore store;
    const auto userId = testUser;
    const auto rsaKeyPair = EncryptionUtils::generateRSAKey();
    const auto priv = rsaKeyPair.publicKey;
    const auto pub = rsaKeyPair.privateKey;

    QVERIFY(store.saveKey(userId, priv, pub));
    QVERIFY(store.hasKey(userId));

    QByteArray loadedPriv, loadedPub;
    QVERIFY(store.loadKey(userId, loadedPriv, loadedPub));
    QCOMPARE(loadedPriv, priv);
    QCOMPARE(loadedPub, pub);

    QVERIFY(store.deleteKey(userId));
    QVERIFY(!store.hasKey(userId));
}

void E2EKeyStoreTest::testOverwrite()
{
    E2EKeyStore store;
    const auto userId = testUser;
    const auto rsaKeyPair1 = EncryptionUtils::generateRSAKey();
    const auto rsaKeyPair2 = EncryptionUtils::generateRSAKey();

    const auto priv1 = rsaKeyPair1.privateKey;
    const auto pub1 = rsaKeyPair1.publicKey;
    const auto priv2 = rsaKeyPair2.privateKey;
    const auto pub2 = rsaKeyPair2.publicKey;

    QVERIFY(store.saveKey(userId, priv1, pub1));
    QVERIFY(store.saveKey(userId, priv2, pub2));

    QByteArray loadedPriv, loadedPub;
    QVERIFY(store.loadKey(userId, loadedPriv, loadedPub));
    QCOMPARE(loadedPriv, priv2);
    QCOMPARE(loadedPub, pub2);

    store.deleteKey(userId);
}

void E2EKeyStoreTest::testNonexistentKey()
{
    E2EKeyStore store;
    const auto userId = otherUser;
    QByteArray priv, pub;
    QVERIFY(!store.hasKey(userId));
    QVERIFY(!store.loadKey(userId, priv, pub));
    QVERIFY(store.deleteKey(userId));
}

#include "moc_e2ekeystoretest.cpp"
