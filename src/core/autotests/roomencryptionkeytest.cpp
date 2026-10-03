/*

   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomencryptionkeytest.h"
#include "config-ruqola.h"
#include "roomencryptionkey.h"
#if USE_E2E_SUPPORT
#include "encryption/encryptionutils.h"
#endif
#include <QJsonArray>
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(RoomEncryptionKeyTest)

RoomEncryptionKeyTest::RoomEncryptionKeyTest(QObject *parent)
    : QObject{parent}
{
}

void RoomEncryptionKeyTest::shouldHaveDefaultValues()
{
    const RoomEncryptionKey w;
    QVERIFY(w.e2EKey().isEmpty());
    QVERIFY(w.e2eKeyId().isEmpty());
    QVERIFY(w.e2ESuggestedKey().isEmpty());
    QVERIFY(w.usersWaitingForE2EKeys().isEmpty());
    QVERIFY(w.sessionKey().isEmpty());
}
// The room key another member shares with us is a 16-byte A128CBC one when the room was keyed
// before Rocket.Chat moved to AES-GCM. Refusing it does not merely hide the room: the caller reads
// the empty session key as an invalid suggestion and asks the server to discard it.
void RoomEncryptionKeyTest::shouldImportLegacyCbcSessionKey()
{
#if USE_E2E_SUPPORT
    const EncryptionUtils::RSAKeyPair keyPair = EncryptionUtils::generateRSAKey();
    QVERIFY(!keyPair.privateKey.isEmpty());
    RSA *publicKey = EncryptionUtils::publicKeyFromPEM(keyPair.publicKey);
    QVERIFY(publicKey);

    const QString keyId = EncryptionUtils::generateRoomKeyId();
    const QByteArray legacySessionKey(16, 'k');
    const QByteArray sharedKey = EncryptionUtils::encryptSessionKey(EncryptionUtils::sessionKeyToJWK(legacySessionKey), publicKey);
    RSA_free(publicKey);
    QVERIFY(!sharedKey.isEmpty());

    RoomEncryptionKey legacyRoomKey;
    legacyRoomKey.setE2EKey(keyId + QString::fromLatin1(sharedKey.toBase64()));
    RSA *privateKey = EncryptionUtils::privateKeyFromPEM(keyPair.privateKey);
    QVERIFY(privateKey);
    legacyRoomKey.decryptWithPrivateKey(privateKey);
    QCOMPARE(legacyRoomKey.e2eKeyId(), keyId);
    QCOMPARE(legacyRoomKey.sessionKey(), legacySessionKey);

    // The current flavour must keep working the same way.
    RSA *ownPublicKey = EncryptionUtils::publicKeyFromPEM(keyPair.publicKey);
    QVERIFY(ownPublicKey);
    const QByteArray sessionKey = EncryptionUtils::generateSessionKey();
    const QByteArray sharedGcmKey = EncryptionUtils::encryptSessionKey(EncryptionUtils::sessionKeyToJWK(sessionKey), ownPublicKey);
    RSA_free(ownPublicKey);
    RoomEncryptionKey roomKey;
    roomKey.setE2EKey(keyId + QString::fromLatin1(sharedGcmKey.toBase64()));
    roomKey.decryptWithPrivateKey(privateKey);
    QCOMPARE(roomKey.sessionKey(), sessionKey);

    RSA_free(privateKey);
#endif
}

// Once a room is re-keyed, the messages written before it keep naming the key of their own era.
// Rocket.Chat hands those keys over in the subscription and reads that history with them, so
// dropping them is what leaves a room half readable.
void RoomEncryptionKeyTest::shouldKeepOldRoomKeysToReadHistory()
{
#if USE_E2E_SUPPORT
    const EncryptionUtils::RSAKeyPair keyPair = EncryptionUtils::generateRSAKey();
    QVERIFY(!keyPair.privateKey.isEmpty());

    const auto shareKeyForKeyId = [&keyPair](const QString &keyId, const QByteArray &sessionKey) -> QString {
        RSA *publicKey = EncryptionUtils::publicKeyFromPEM(keyPair.publicKey);
        const QByteArray encrypted = EncryptionUtils::encryptSessionKey(EncryptionUtils::sessionKeyToJWK(sessionKey), publicKey);
        RSA_free(publicKey);
        return keyId + QString::fromLatin1(encrypted.toBase64());
    };

    const QString currentKeyId = EncryptionUtils::generateRoomKeyId();
    const QString previousKeyId = EncryptionUtils::generateRoomKeyId();
    const QString suggestedKeyId = EncryptionUtils::generateRoomKeyId();
    const QByteArray currentSessionKey = EncryptionUtils::generateSessionKey();
    const QByteArray previousSessionKey = EncryptionUtils::generateSessionKey();
    const QByteArray suggestedSessionKey = EncryptionUtils::generateSessionKey();

    RoomEncryptionKey roomKey;
    QVERIFY(!roomKey.hasEncryptedKeys());
    roomKey.setE2EKey(shareKeyForKeyId(currentKeyId, currentSessionKey));

    QJsonObject oldKey;
    oldKey[u"e2eKeyId"_s] = previousKeyId;
    oldKey[u"E2EKey"_s] = shareKeyForKeyId(previousKeyId, previousSessionKey);
    // The server hands the suggested ones over in a separate field, both go through the same parse.
    QJsonObject suggestedOldKey;
    suggestedOldKey[u"e2eKeyId"_s] = suggestedKeyId;
    suggestedOldKey[u"E2EKey"_s] = shareKeyForKeyId(suggestedKeyId, suggestedSessionKey);
    roomKey.parseOldRoomKeys(QJsonArray() << oldKey);
    roomKey.parseOldRoomKeys(QJsonArray() << suggestedOldKey);
    QVERIFY(roomKey.hasEncryptedKeys());
    QVERIFY(!roomKey.hasSessionKey());

    RSA *privateKey = EncryptionUtils::privateKeyFromPEM(keyPair.privateKey);
    QVERIFY(privateKey);
    roomKey.decryptWithPrivateKey(privateKey);
    RSA_free(privateKey);

    QVERIFY(roomKey.hasSessionKey());
    QCOMPARE(roomKey.sessionKey(), currentSessionKey);
    QCOMPARE(roomKey.sessionKeyForKeyId(currentKeyId), currentSessionKey);
    QCOMPARE(roomKey.sessionKeyForKeyId(previousKeyId), previousSessionKey);
    QCOMPARE(roomKey.sessionKeyForKeyId(suggestedKeyId), suggestedSessionKey);
    // An unknown or absent kid can only be tried with the current key, as Rocket.Chat does.
    QCOMPARE(roomKey.sessionKeyForKeyId(EncryptionUtils::generateRoomKeyId()), currentSessionKey);
    QCOMPARE(roomKey.sessionKeyForKeyId({}), currentSessionKey);

    // Re-parsing the same list must neither duplicate a key nor lose the session key we decrypted,
    // and the current key never belongs to the old ones.
    roomKey.parseOldRoomKeys(QJsonArray() << oldKey << oldKey);
    QJsonObject currentAsOldKey;
    currentAsOldKey[u"e2eKeyId"_s] = currentKeyId;
    currentAsOldKey[u"E2EKey"_s] = shareKeyForKeyId(currentKeyId, EncryptionUtils::generateSessionKey());
    roomKey.parseOldRoomKeys(QJsonArray() << currentAsOldKey);
    QCOMPARE(roomKey.sessionKeyForKeyId(previousKeyId), previousSessionKey);
    QCOMPARE(roomKey.sessionKeyForKeyId(currentKeyId), currentSessionKey);
#endif
}

// When the room announces another key id, the key we hold is not the room key any more. Nothing
// downstream compares the two, so keeping it would mean decrypting with a retired key and — far
// worse — encrypting outgoing messages with it under the new id, which nobody could ever read.
void RoomEncryptionKeyTest::shouldRetireTheCurrentKeyWhenTheRoomIsRekeyed()
{
#if USE_E2E_SUPPORT
    const EncryptionUtils::RSAKeyPair keyPair = EncryptionUtils::generateRSAKey();
    QVERIFY(!keyPair.privateKey.isEmpty());
    RSA *publicKey = EncryptionUtils::publicKeyFromPEM(keyPair.publicKey);
    QVERIFY(publicKey);
    const QString firstKeyId = EncryptionUtils::generateRoomKeyId();
    const QByteArray firstSessionKey = EncryptionUtils::generateSessionKey();
    const QByteArray sharedKey = EncryptionUtils::encryptSessionKey(EncryptionUtils::sessionKeyToJWK(firstSessionKey), publicKey);
    RSA_free(publicKey);

    RoomEncryptionKey roomKey;
    roomKey.setE2EKey(firstKeyId + QString::fromLatin1(sharedKey.toBase64()));
    RSA *privateKey = EncryptionUtils::privateKeyFromPEM(keyPair.privateKey);
    QVERIFY(privateKey);
    roomKey.decryptWithPrivateKey(privateKey);
    RSA_free(privateKey);
    QCOMPARE(roomKey.sessionKey(), firstSessionKey);

    // The room was re-keyed by another member: the new id arrives before its key does.
    const QString secondKeyId = EncryptionUtils::generateRoomKeyId();
    roomKey.setE2eKeyId(secondKeyId);
    QCOMPARE(roomKey.e2eKeyId(), secondKeyId);
    // No key for the new id, so nothing is encrypted or decrypted with the retired one...
    QVERIFY(roomKey.sessionKey().isEmpty());
    QVERIFY(roomKey.sessionKeyForKeyId(secondKeyId).isEmpty());
    QVERIFY(roomKey.e2EKey().isEmpty());
    // ...but the history written under the first id stays readable.
    QCOMPARE(roomKey.sessionKeyForKeyId(firstKeyId), firstSessionKey);
    QVERIFY(roomKey.hasSessionKey());
    QVERIFY(roomKey.hasEncryptedKeys());

    // Learning the id for the first time retires nothing, and neither does a payload which says
    // nothing about it.
    {
        RoomEncryptionKey freshRoomKey;
        freshRoomKey.setE2EKey(firstKeyId + QString::fromLatin1(sharedKey.toBase64()));
        QCOMPARE(freshRoomKey.e2eKeyId(), firstKeyId);
        freshRoomKey.setE2eKeyId(firstKeyId);
        QCOMPARE(freshRoomKey.e2EKey(), firstKeyId + QString::fromLatin1(sharedKey.toBase64()));
        freshRoomKey.setE2eKeyId({});
        QVERIFY(!freshRoomKey.e2EKey().isEmpty());
    }
#endif
}

#include "moc_roomencryptionkeytest.cpp"
