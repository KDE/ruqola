/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sessionkeytest.h"
#include "encryption/encryptionutils.h"
#include <QTest>

QTEST_GUILESS_MAIN(SessionKeyTest)
SessionKeyTest::SessionKeyTest(QObject *parent)
    : QObject(parent)
{
}

void SessionKeyTest::sessionKeyGenerationTest()
{
    QVERIFY(!(EncryptionUtils::generateSessionKey().isEmpty()));
}

/**
 * @brief Tests the encryption and decryption of the session key.
 *
 * This test verifies that a randomly generated 128-bit (16 bytes) AES session key,
 * when encrypted with an RSA public key and then decrypted with the corresponding
 * RSA private key, results in the original session key.
 */
void SessionKeyTest::sessionKeyEncryptionDecryptionTest()
{
    QByteArray sessionKey;
    QByteArray encryptedSessionKey;
    QByteArray decryptedSessionKey;
    auto rsaKeyPair = EncryptionUtils::generateRSAKey();
    auto privateKey = rsaKeyPair.privateKey;
    auto publicKey = rsaKeyPair.publicKey;

    for (int i = 0; i <= 10; i++) {
        sessionKey = EncryptionUtils::generateSessionKey();
        encryptedSessionKey = EncryptionUtils::encryptSessionKey(sessionKey, EncryptionUtils::publicKeyFromPEM(publicKey));
        decryptedSessionKey = EncryptionUtils::decryptSessionKey(encryptedSessionKey, EncryptionUtils::privateKeyFromPEM(privateKey));
        QVERIFY(sessionKey == decryptedSessionKey);
    }
}

#include "moc_sessionkeytest.cpp"
