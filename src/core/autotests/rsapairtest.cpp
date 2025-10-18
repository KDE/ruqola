/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rsapairtest.h"
#include "encryption/encryptionutils.h"
#include <QTest>

QTEST_GUILESS_MAIN(RsaPairTest)
RsaPairTest::RsaPairTest(QObject *parent)
    : QObject(parent)
{
}

void RsaPairTest::rsaPairGenerationNonDeterminismTest()
{
    EncryptionUtils::RSAKeyPair rsaPair1;
    EncryptionUtils::RSAKeyPair rsaPair2;

    for (int i = 0; i <= 10; i++) {
        rsaPair1 = EncryptionUtils::generateRSAKey();
        rsaPair2 = EncryptionUtils::generateRSAKey();
        QVERIFY(rsaPair1.publicKey != rsaPair2.publicKey);
        QVERIFY(rsaPair1.privateKey != rsaPair2.privateKey);
    }
}

/**
 * @brief Tests the determinism of private key encryption and decryption using the master key.
 *
 * Definitions:
 *   - x = master key
 *
 *   - y = initial private key
 *
 *   - z = encrypt(x, y) = encrypted private key
 *
 *   - w = decrypt(x, z) = decrypted private key
 *
 * The test verifies:
 *
 *   If the same master key x and private key y are used,
 *     then decrypting the encrypted key yields the original key:
 *
 *   - decrypt(x, encrypt(x, y)) = y = initial private key
 *
 *   In other words, w = y iff x and y are unchanged.
 */
void RsaPairTest::encryptDecryptDeterminismTest()
{
    EncryptionUtils::RSAKeyPair rsaPair;
    QByteArray privateKey;
    QByteArray masterKey;
    QByteArray encryptedPrivateKey;
    QByteArray decryptedPrivateKey;

    for (int i = 0; i <= 10; i++) {
        rsaPair = EncryptionUtils::generateRSAKey();
        privateKey = rsaPair.privateKey;
        masterKey = EncryptionUtils::getMasterKey(EncryptionUtils::generateRandomText(32), EncryptionUtils::generateRandomText(32));
        encryptedPrivateKey = EncryptionUtils::encryptPrivateKey(rsaPair.privateKey, masterKey);
        decryptedPrivateKey = EncryptionUtils::decryptPrivateKey(encryptedPrivateKey, masterKey);

        QCOMPARE(decryptedPrivateKey, privateKey);
    }
}

#include "moc_rsapairtest.cpp"
