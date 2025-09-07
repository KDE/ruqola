/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageencryptiondecryptiontest.h"
#include "encryption/encryptionutils.h"
#include <QTest>

QTEST_GUILESS_MAIN(MessageEncryptionDecryptionTest)
MessageEncryptionDecryptionTest::MessageEncryptionDecryptionTest(QObject *parent)
    : QObject(parent)
{
}

/**
 * @brief Tests symmetric encryption and decryption of a message using a session key.
 *
 * Verifies that for a message `m` and session key `k`, decryption function `D`
 * and encryption function `E` the property holds:
 *
 *     `D(E(m, k), k) = m`
 */
void MessageEncryptionDecryptionTest::messageEncryptionDecryptionTest()
{
    auto message = QStringLiteral("This is GSoC 2025, Andro Ranogajec got to the end of 'Phase 1' :)");
    const QByteArray sessionKey1 = EncryptionUtils::generateSessionKey();
    const QByteArray sessionKey2 = EncryptionUtils::generateSessionKey();
    QString decryptedMessage = QString::fromUtf8(EncryptionUtils::decryptMessage(EncryptionUtils::encryptMessage(message.toUtf8(), sessionKey1), sessionKey1));
    QVERIFY(message == decryptedMessage);

    for (int i = 1; i <= 10; ++i) {
        QByteArray message = EncryptionUtils::generateRandomText(i).toUtf8();
        QByteArray encrypted = EncryptionUtils::encryptMessage(message, sessionKey1);
        QByteArray decryptedWithWrongKey = EncryptionUtils::decryptMessage(encrypted, sessionKey2);
        QVERIFY(decryptedWithWrongKey.isEmpty() && decryptedWithWrongKey != message);
    }
}

#include "moc_messageencryptiondecryptiontest.cpp"
