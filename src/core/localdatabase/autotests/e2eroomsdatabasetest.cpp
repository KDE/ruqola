/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "e2eroomsdatabasetest.h"
#include "encryption/encryptionutils.h"
#include "localdatabase/e2eroomsdatabase.h"
#include <QTest>

QTEST_GUILESS_MAIN(E2ERoomsDataBaseTest)
using namespace Qt::Literals::StringLiterals;
const auto testRoom = QStringLiteral("testRoom");
const auto testKey = QStringLiteral("testKey");
const auto testOtherRoom = QStringLiteral("testOtherRoom");
const auto testOtherKey = QStringLiteral("testOtherKey");
static QString accountName()
{
    return u"myAccount"_s;
}
using namespace Qt::Literals::StringLiterals;
void E2ERoomsDataBaseTest::initTestCase()
{
    E2ERoomsDataBase store;
    QVERIFY(store.deleteKey(accountName(), testRoom, testKey));
    QVERIFY(store.deleteKey(accountName(), testOtherRoom, testOtherKey));
}

void E2ERoomsDataBaseTest::shouldDefaultValues()
{
    E2ERoomsDataBase E2ERoomsDataBase;
    QCOMPARE(E2ERoomsDataBase.schemaDatabaseStr(),
             u"CREATE TABLE E2EROOMSKEYS (roomKeyId TEXT PRIMARY KEY NOT NULL, encryptedPrivateKey BLOB, publicKey BLOB)"_s);
}

void E2ERoomsDataBaseTest::testSaveLoadDelete()
{
    E2ERoomsDataBase store;
    const auto userId = testRoom;
    const auto rsaKeyPair = EncryptionUtils::generateRSAKey();
    const auto priv = rsaKeyPair.publicKey;
    const auto pub = rsaKeyPair.privateKey;

    QVERIFY(store.saveKey(accountName(), testRoom, testKey, priv, pub));
    QVERIFY(store.hasKey(accountName(), testRoom, testKey));

    QByteArray loadedPriv;
    QByteArray loadedPub;
    QVERIFY(store.loadKey(accountName(), testRoom, testKey, loadedPriv, loadedPub));
    QCOMPARE(loadedPriv, priv);
    QCOMPARE(loadedPub, pub);

    QVERIFY(store.deleteKey(accountName(), testRoom, testKey));
    QVERIFY(!store.hasKey(accountName(), testRoom, testKey));
}

void E2ERoomsDataBaseTest::testOverwrite()
{
    E2ERoomsDataBase store;
    const auto userId = testRoom;
    const auto rsaKeyPair1 = EncryptionUtils::generateRSAKey();
    const auto rsaKeyPair2 = EncryptionUtils::generateRSAKey();

    const auto priv1 = rsaKeyPair1.privateKey;
    const auto pub1 = rsaKeyPair1.publicKey;
    const auto priv2 = rsaKeyPair2.privateKey;
    const auto pub2 = rsaKeyPair2.publicKey;

    QVERIFY(store.saveKey(accountName(), testRoom, testKey, priv1, pub1));
    {
        QByteArray loadedPriv, loadedPub;
        QVERIFY(store.loadKey(accountName(), testRoom, testKey, loadedPriv, loadedPub));
        QCOMPARE(loadedPriv, priv1);
        QCOMPARE(loadedPub, pub1);
    }

    QVERIFY(store.saveKey(accountName(), testRoom, testKey, priv2, pub2));
    {
        QByteArray loadedPriv, loadedPub;
        QVERIFY(store.loadKey(accountName(), testRoom, testKey, loadedPriv, loadedPub));
        QCOMPARE(loadedPriv, priv2);
        QCOMPARE(loadedPub, pub2);
    }
    QVERIFY(store.deleteKey(accountName(), testRoom, testKey));
}

void E2ERoomsDataBaseTest::testNonExistentKey()
{
    E2ERoomsDataBase store;
    const auto userId = testOtherRoom;
    QByteArray priv, pub;
    QVERIFY(!store.hasKey(accountName(), testRoom, testKey));
    QVERIFY(!store.loadKey(accountName(), testRoom, testKey, priv, pub));
    QVERIFY(store.deleteKey(accountName(), testRoom, testKey));
}

#include "moc_e2eroomsdatabasetest.cpp"
