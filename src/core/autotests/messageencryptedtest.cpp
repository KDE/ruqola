/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messageencryptedtest.h"
#include "config-ruqola.h"
#include "messages/messageencrypted.h"
#include <QTest>

QTEST_GUILESS_MAIN(MessageEncryptedTest)

MessageEncryptedTest::MessageEncryptedTest(QObject *parent)
    : QObject{parent}
{
}

void MessageEncryptedTest::shouldHaveDefaultValues()
{
    const MessageEncrypted w;
    QVERIFY(w.algorithm().isEmpty());
    QVERIFY(w.ciphertext().isEmpty());
    QVERIFY(w.keyId().isEmpty());
    QVERIFY(w.iv().isEmpty());
    QVERIFY(!w.isValid());
}

void MessageEncryptedTest::shouldEncryptV2Payload()
{
    const QByteArray sessionKey(32, 'k');
    const QByteArray plainText("{\"msg\":\"hello e2e\"}");
    const QByteArray keyId("23e2720d-b3e0-4753-85ff-bad2caeb867b");

    MessageEncrypted encrypted;
    const bool encryptedOk = encrypted.encrypt(plainText, sessionKey, keyId);
    if (encryptedOk) {
        QCOMPARE(encrypted.algorithm(), QByteArray("rc.v2.aes-sha2"));
        QCOMPARE(encrypted.keyId(), keyId);
        QVERIFY(!QByteArray::fromBase64(encrypted.iv()).isEmpty());
        QVERIFY(!QByteArray::fromBase64(encrypted.ciphertext().toLatin1()).isEmpty());
        QCOMPARE(encrypted.decrypt(sessionKey), plainText);
    } else {
        QVERIFY(!encrypted.isValid());
    }
}

void MessageEncryptedTest::shouldDecryptV2Payload()
{
    // Test vector generated with AES-256-GCM, key='k'*32, iv="0123456789ab", plaintext={"msg":"hello e2e"}
    const QByteArray sessionKey(32, 'k');
    const QByteArray iv("0123456789ab");
    const QByteArray plainText("{\"msg\":\"hello e2e\"}");
    const QByteArray encryptedPayload = QByteArray::fromBase64("ej0KsqEKP7tIhPFauxLZfLCDiI6PY2Ex68Kv4kt2sCFIA24=");
    QVERIFY(!encryptedPayload.isEmpty());

    MessageEncrypted encrypted;
    encrypted.setAlgorithm("rc.v2.aes-sha2");
    encrypted.setKeyId("23e2720d-b3e0-4753-85ff-bad2caeb867b");
    encrypted.setIv(iv.toBase64());
    encrypted.setCiphertext(QString::fromLatin1(encryptedPayload.toBase64()));

    const QByteArray decryptedPayload = encrypted.decrypt(sessionKey);
    QVERIFY(decryptedPayload.isEmpty() || decryptedPayload == plainText);

    const QByteArray wrongSessionKey(32, 'x');
    QVERIFY(encrypted.decrypt(wrongSessionKey).isEmpty());
}

// A room keyed before Rocket.Chat switched to AES-GCM still holds a 16-byte A128CBC key, and its
// clients encrypt with AES-CBC-128 even inside a "rc.v2.aes-sha2" content: the mode follows the
// key, not the content version.
void MessageEncryptedTest::shouldEncryptLegacyCbcPayload()
{
    const QByteArray sessionKey(16, 'k');
    const QByteArray plainText("{\"msg\":\"hello legacy\"}");
    const QByteArray keyId("23e2720d-b3e0-4753-85ff-bad2caeb867b");

    MessageEncrypted encrypted;
    const bool encryptedOk = encrypted.encrypt(plainText, sessionKey, keyId);
#if USE_E2E_SUPPORT
    QVERIFY(encryptedOk);
    QCOMPARE(encrypted.algorithm(), QByteArray("rc.v2.aes-sha2"));
    QCOMPARE(encrypted.keyId(), keyId);
    // AES-CBC takes a full-block IV, unlike the 12-byte one of AES-GCM.
    QCOMPARE(QByteArray::fromBase64(encrypted.iv()).size(), 16);
    QCOMPARE(encrypted.decrypt(sessionKey), plainText);

    const QByteArray wrongSessionKey(16, 'x');
    QVERIFY(encrypted.decrypt(wrongSessionKey) != plainText);

    // A key of a size no Rocket.Chat client can import must not be used at all.
    MessageEncrypted bogus;
    QVERIFY(!bogus.encrypt(plainText, QByteArray(24, 'k'), keyId));
#else
    QVERIFY(!encryptedOk);
#endif
}

void MessageEncryptedTest::shouldDecryptLegacyCbcPayload()
{
    // Test vector generated with AES-128-CBC, key='k'*16, iv="0123456789abcdef"
    const QByteArray sessionKey(16, 'k');
    const QByteArray iv("0123456789abcdef");
    const QByteArray plainText("{\"msg\":\"hello e2e\"}");
    const QByteArray encryptedPayload = QByteArray::fromBase64("mz5PnpxdrRzYBBDBt7J3k9xh/1aFGvdiki4lnf6aSYM=");
    QVERIFY(!encryptedPayload.isEmpty());

    MessageEncrypted encrypted;
    encrypted.setAlgorithm("rc.v2.aes-sha2");
    encrypted.setKeyId("23e2720d-b3e0-4753-85ff-bad2caeb867b");
    encrypted.setIv(iv.toBase64());
    encrypted.setCiphertext(QString::fromLatin1(encryptedPayload.toBase64()));

#if USE_E2E_SUPPORT
    QCOMPARE(encrypted.decrypt(sessionKey), plainText);
    QVERIFY(encrypted.decrypt(QByteArray(16, 'x')) != plainText);
#else
    QVERIFY(encrypted.decrypt(sessionKey).isEmpty());
#endif
}

// A "rc.v1.aes-sha2" message has no {kid, iv, ciphertext} object: the whole payload is the single
// string "kid(12) + base64(iv[16] + ciphertext)". Normalising it into the fields of the current
// format is what lets one decryption path and one key lookup serve both versions.
void MessageEncryptedTest::shouldParseLegacyV1Payload()
{
    const QByteArray iv("0123456789abcdef");
    const QByteArray sessionKey(16, 'k');
    const QByteArray plainText("{\"msg\":\"hello e2e\"}");
    // Same AES-128-CBC vector as above, with the iv prepended and a 12-character key id in front.
    const QString keyId = QStringLiteral("a1b2c3d4e5f6");
    const QString payload = keyId + QStringLiteral("MDEyMzQ1Njc4OWFiY2RlZps+T56cXa0c2AQQwbeyd5PcYf9WhRr3YpIuJZ3+mkmD");

    MessageEncrypted encrypted;
    QVERIFY(encrypted.parseLegacyPayload(payload));
    QCOMPARE(encrypted.algorithm(), QByteArray("rc.v1.aes-sha2"));
    // The key id has to be readable, it is what selects the room key of that era.
    QCOMPARE(encrypted.keyId(), keyId.toLatin1());
    QCOMPARE(QByteArray::fromBase64(encrypted.iv()), iv);
    QVERIFY(encrypted.isValid());
#if USE_E2E_SUPPORT
    QCOMPARE(encrypted.decrypt(sessionKey), plainText);
#endif

    // Payloads that cannot carry a key id, or an iv and a ciphertext, must be refused rather than
    // decoded into something plausible-looking.
    MessageEncrypted toooShort;
    QVERIFY(!toooShort.parseLegacyPayload(QStringLiteral("a1b2c3d4e5f6")));
    QVERIFY(!toooShort.parseLegacyPayload({}));
    QVERIFY(!toooShort.parseLegacyPayload(keyId + QString::fromLatin1(iv.toBase64())));
    QVERIFY(!toooShort.isValid());
}

#include "moc_messageencryptedtest.cpp"
