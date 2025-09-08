/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "e2edatabasetest.h"
#include "encryption/encryptionutils.h"
#include "localdatabase/e2edatabase.h"
#include <QtTest>

QTEST_GUILESS_MAIN(E2EDataBaseTest)

const auto testUser = QStringLiteral("testuser");
const auto otherUser = QStringLiteral("otheruser");
using namespace Qt::Literals::StringLiterals;
void E2EDataBaseTest::initTestCase()
{
    E2EDataBase store;
    store.deleteKey(testUser);
    store.deleteKey(otherUser);
}

void E2EDataBaseTest::shouldDefaultValues()
{
    E2EDataBase e2eDatabase;
    QCOMPARE(e2eDatabase.schemaDatabaseStr(), u"CREATE TABLE E2EKEYS (userId TEXT PRIMARY KEY NOT NULL, encryptedPrivateKey BLOB, publicKey BLOB)"_s);
}

void E2EDataBaseTest::testSaveLoadDelete()
{
    E2EDataBase store;
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

void E2EDataBaseTest::testOverwrite()
{
    E2EDataBase store;
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

void E2EDataBaseTest::testNonexistentKey()
{
    E2EDataBase store;
    const auto userId = otherUser;
    QByteArray priv, pub;
    QVERIFY(!store.hasKey(userId));
    QVERIFY(!store.loadKey(userId, priv, pub));
    QVERIFY(store.deleteKey(userId));
}

#include "moc_e2edatabasetest.cpp"
