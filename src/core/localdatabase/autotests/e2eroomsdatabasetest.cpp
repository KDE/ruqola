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
const auto testUser = QStringLiteral("testuser");
const auto otherUser = QStringLiteral("otheruser");
static QString accountName()
{
    return u"myAccount"_s;
}
using namespace Qt::Literals::StringLiterals;
void E2ERoomsDataBaseTest::initTestCase()
{
    E2ERoomsDataBase store;
    QVERIFY(store.deleteKey(accountName(), testUser));
    QVERIFY(store.deleteKey(accountName(), otherUser));
}

void E2ERoomsDataBaseTest::shouldDefaultValues()
{
    E2ERoomsDataBase E2ERoomsDataBase;
    QCOMPARE(E2ERoomsDataBase.schemaDatabaseStr(), u"CREATE TABLE E2EKEYS (userId TEXT PRIMARY KEY NOT NULL, encryptedPrivateKey BLOB, publicKey BLOB)"_s);
}

void E2ERoomsDataBaseTest::testSaveLoadDelete()
{
    E2ERoomsDataBase store;
    const auto userId = testUser;
    const auto rsaKeyPair = EncryptionUtils::generateRSAKey();
    const auto priv = rsaKeyPair.publicKey;
    const auto pub = rsaKeyPair.privateKey;

    QVERIFY(store.saveKey(accountName(), userId, priv, pub));
    QVERIFY(store.hasKey(accountName(), userId));

    QByteArray loadedPriv;
    QByteArray loadedPub;
    QVERIFY(store.loadKey(accountName(), userId, loadedPriv, loadedPub));
    QCOMPARE(loadedPriv, priv);
    QCOMPARE(loadedPub, pub);

    QVERIFY(store.deleteKey(accountName(), userId));
    QVERIFY(!store.hasKey(accountName(), userId));
}

void E2ERoomsDataBaseTest::testOverwrite()
{
    E2ERoomsDataBase store;
    const auto userId = testUser;
    const auto rsaKeyPair1 = EncryptionUtils::generateRSAKey();
    const auto rsaKeyPair2 = EncryptionUtils::generateRSAKey();

    const auto priv1 = rsaKeyPair1.privateKey;
    const auto pub1 = rsaKeyPair1.publicKey;
    const auto priv2 = rsaKeyPair2.privateKey;
    const auto pub2 = rsaKeyPair2.publicKey;

    QVERIFY(store.saveKey(accountName(), userId, priv1, pub1));
    {
        QByteArray loadedPriv, loadedPub;
        QVERIFY(store.loadKey(accountName(), userId, loadedPriv, loadedPub));
        QCOMPARE(loadedPriv, priv1);
        QCOMPARE(loadedPub, pub1);
    }

    QVERIFY(store.saveKey(accountName(), userId, priv2, pub2));
    {
        QByteArray loadedPriv, loadedPub;
        QVERIFY(store.loadKey(accountName(), userId, loadedPriv, loadedPub));
        QCOMPARE(loadedPriv, priv2);
        QCOMPARE(loadedPub, pub2);
    }
    QVERIFY(store.deleteKey(accountName(), userId));
}

void E2ERoomsDataBaseTest::testNonExistentKey()
{
    E2ERoomsDataBase store;
    const auto userId = otherUser;
    QByteArray priv, pub;
    QVERIFY(!store.hasKey(accountName(), userId));
    QVERIFY(!store.loadKey(accountName(), userId, priv, pub));
    QVERIFY(store.deleteKey(accountName(), userId));
}

#include "moc_e2eroomsdatabasetest.cpp"
