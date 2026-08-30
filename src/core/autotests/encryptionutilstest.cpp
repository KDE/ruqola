/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "encryptionutilstest.h"
#include "encryption/encryptionutils.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(EncryptionUtilsTest)
EncryptionUtilsTest::EncryptionUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void EncryptionUtilsTest::shouldExportJWKKey()
{
    // TODO
}

// A generated key pair has to be published in the format every Rocket.Chat client can read back:
// JWK for the public key, and a JWK private key sealed in the "V2" envelope.
void EncryptionUtilsTest::shouldRoundTripGeneratedKeyPairAsRocketChatDoes()
{
    const EncryptionUtils::RSAKeyPair keyPair = EncryptionUtils::generateRSAKey();
    QVERIFY(!keyPair.privateKey.isEmpty());

    RSA *rsa = EncryptionUtils::privateKeyFromPEM(keyPair.privateKey);
    QVERIFY(rsa);
    const QByteArray publicKeyJwk = EncryptionUtils::exportJWKPublicKey(rsa);
    const QByteArray privateKeyJwk = EncryptionUtils::exportJWKPrivateKey(rsa);
    RSA_free(rsa);

    // Both must be JWK JSON: a PEM would make the other clients throw when importing them.
    const QJsonObject publicKeyObject = QJsonDocument::fromJson(publicKeyJwk).object();
    QCOMPARE(publicKeyObject.value(u"kty"_s).toString(), u"RSA"_s);
    QVERIFY(!publicKeyObject.value(u"n"_s).toString().isEmpty());

    // We store that JWK locally and encrypt the room keys with it, so it must convert back.
    RSA *publicKey = EncryptionUtils::publicKeyFromPEM(EncryptionUtils::publicKeyJWKToPEM(publicKeyJwk));
    QVERIFY(publicKey);
    RSA_free(publicKey);

    const QJsonObject privateKeyObject = QJsonDocument::fromJson(privateKeyJwk).object();
    QCOMPARE(privateKeyObject.value(u"kty"_s).toString(), u"RSA"_s);
    for (const QString &component : {u"n"_s, u"e"_s, u"d"_s, u"p"_s, u"q"_s, u"dp"_s, u"dq"_s, u"qi"_s}) {
        QVERIFY2(!privateKeyObject.value(component).toString().isEmpty(), qPrintable(component));
    }
    // The JWK must describe the very same key.
    QCOMPARE(EncryptionUtils::privateKeyJWKToPEM(privateKeyJwk), keyPair.privateKey);

    const QString password = u"secret password"_s;
    const QString userId = u"userId"_s;
    const QByteArray storedKey = EncryptionUtils::encryptPrivateKeyV2(privateKeyJwk, password, userId);
    const QJsonObject storedKeyObject = QJsonDocument::fromJson(storedKey).object();
    QVERIFY(storedKeyObject.value(u"salt"_s).toString().startsWith(u"v2:"_s + userId + u':'));
    QCOMPARE(storedKeyObject.value(u"iterations"_s).toInt(), 100000);

    // Decrypting it the way every client does must give the JWK private key back. A 16-byte IV
    // would be understood as the legacy AES-CBC layout, so it has to be shorter.
    const QByteArray iv = QByteArray::fromBase64(storedKeyObject.value(u"iv"_s).toString().toUtf8());
    QCOMPARE(iv.size(), 12);
    const QByteArray masterKey =
        EncryptionUtils::deriveKey(storedKeyObject.value(u"salt"_s).toString().toUtf8(), password.toUtf8(), storedKeyObject.value(u"iterations"_s).toInt(), 32);
    const QByteArray ciphertext = QByteArray::fromBase64(storedKeyObject.value(u"ciphertext"_s).toString().toUtf8());
    QCOMPARE(EncryptionUtils::decryptAES_GCM_256(ciphertext, masterKey, iv), privateKeyJwk);

    // A wrong password must not decrypt it.
    const QByteArray wrongMasterKey = EncryptionUtils::deriveKey(storedKeyObject.value(u"salt"_s).toString().toUtf8(),
                                                                 "other password"_ba,
                                                                 storedKeyObject.value(u"iterations"_s).toInt(),
                                                                 32);
    QVERIFY(EncryptionUtils::decryptAES_GCM_256(ciphertext, wrongMasterKey, iv).isEmpty());
}

// Rocket.Chat imports a shared room key through ALGORITHM_MAP[jwk.alg], so re-sharing the 16-byte
// key of a legacy room has to keep announcing it as A128CBC: sending it as A256GCM would make the
// recipient import the wrong cipher and read nothing.
void EncryptionUtilsTest::shouldExportSessionKeyJwkForBothAesFlavours()
{
    const QJsonObject gcmJwk = QJsonDocument::fromJson(EncryptionUtils::sessionKeyToJWK(QByteArray(32, 'k'))).object();
    QCOMPARE(gcmJwk.value(u"alg"_s).toString(), u"A256GCM"_s);
    QCOMPARE(gcmJwk.value(u"kty"_s).toString(), u"oct"_s);
    QCOMPARE(QByteArray::fromBase64(gcmJwk.value(u"k"_s).toString().toLatin1(), QByteArray::Base64UrlEncoding), QByteArray(32, 'k'));

    const QJsonObject cbcJwk = QJsonDocument::fromJson(EncryptionUtils::sessionKeyToJWK(QByteArray(16, 'k'))).object();
    QCOMPARE(cbcJwk.value(u"alg"_s).toString(), u"A128CBC"_s);
    QCOMPARE(QByteArray::fromBase64(cbcJwk.value(u"k"_s).toString().toLatin1(), QByteArray::Base64UrlEncoding), QByteArray(16, 'k'));

    // Those two are the only flavours Rocket.Chat knows: anything else has to be refused.
    QVERIFY(EncryptionUtils::sessionKeyToJWK(QByteArray(24, 'k')).isEmpty());
    QVERIFY(EncryptionUtils::sessionKeyToJWK({}).isEmpty());
}

// Rocket.Chat derives the master key from the code units of the password, one byte each, and not
// from its UTF-8 encoding. The two agree for ASCII only, so an accented password sealed the UTF-8
// way produces a private key no other client can unlock.
void EncryptionUtilsTest::shouldDeriveMasterKeyTheWayRocketChatDoes()
{
    // 'ä' and 'ö' are one byte each, as Rocket.Chat's Binary.decode() reads them.
    const QString password = u"p\u00E4ssw\u00F6rd"_s;
    QCOMPARE(EncryptionUtils::keyDerivationBytes(password), QByteArray::fromHex("70e4737377f67264"));
    QVERIFY(EncryptionUtils::keyDerivationBytes(password) != password.toUtf8());
    // ASCII is where the two encodings agree.
    QCOMPARE(EncryptionUtils::keyDerivationBytes(u"password"_s), "password"_ba);

    // A character that does not fit in a byte makes Rocket.Chat throw, so no key may be derived
    // from it: silently folding it into '?' would seal the key with something nothing reproduces.
    QVERIFY(EncryptionUtils::keyDerivationBytes(u"pass\u20ACword"_s).isEmpty());
    QVERIFY(EncryptionUtils::deriveMasterKey(u"v2:userId:1234"_s, u"pass\u20ACword"_s, 1000).isEmpty());

    // Known answer: PBKDF2-HMAC-SHA256 over those bytes.
    const QString salt = u"v2:userId:1234"_s;
    QCOMPARE(EncryptionUtils::deriveMasterKey(salt, password, 1000), QByteArray::fromHex("f8ee28a74439d92a44ebf0970667f33b27fe7e6db5d8561006cdb1bba53d67fc"));
    // What the UTF-8 encoding would have given, i.e. what must no longer be produced.
    QVERIFY(EncryptionUtils::deriveMasterKey(salt, password, 1000) != QByteArray::fromHex("da6df525f5f57608882ff8dba32211bebd86fb710d8612566d61a6062652f79a"));

    // getMasterKey() is the same derivation with the V1 parameters.
    QCOMPARE(EncryptionUtils::getMasterKey(password, salt), EncryptionUtils::deriveMasterKey(salt, password, 1000));
    QVERIFY(EncryptionUtils::deriveMasterKey(salt, {}, 1000).isEmpty());
    QVERIFY(EncryptionUtils::deriveMasterKey({}, password, 1000).isEmpty());
}

void EncryptionUtilsTest::shouldSplitVectorAndEcryptedData_data()
{
    QTest::addColumn<QByteArray>("encryptedData");
    QTest::addColumn<EncryptionUtils::EncryptionInfo>("encryptionInfo");

    {
        const EncryptionUtils::EncryptionInfo info;
        QTest::addRow("empty") << QByteArray() << info;
    }
    {
        const EncryptionUtils::EncryptionInfo info;
        QTest::addRow("too-short") << "1234567890123456"_ba << info;
    }
    {
        EncryptionUtils::EncryptionInfo info;
        info.vector = "1234567890abcdef"_ba;
        info.encryptedData = "cipher-payload"_ba;
        QTest::addRow("iv-and-payload") << QByteArray(info.vector + info.encryptedData) << info;
    }
}

void EncryptionUtilsTest::shouldSplitVectorAndEcryptedData()
{
    QFETCH(QByteArray, encryptedData);
    QFETCH(EncryptionUtils::EncryptionInfo, encryptionInfo);
    QCOMPARE(EncryptionUtils::splitVectorAndEcryptedData(encryptedData), encryptionInfo);
}

void EncryptionUtilsTest::shouldJoinVectorAndEcryptedData_data()
{
    QTest::addColumn<EncryptionUtils::EncryptionInfo>("encryptionInfo");
    QTest::addColumn<QByteArray>("encryptedData");

    {
        const EncryptionUtils::EncryptionInfo info;
        QTest::addRow("empty") << info << QByteArray();
    }
    {
        EncryptionUtils::EncryptionInfo info;
        info.encryptedData = "blafoo-z"_ba;
        info.vector = "AAAPPLLLAPPPAPAPPAPA"_ba;
        QTest::addRow("test1") << info << QByteArray(info.vector + info.encryptedData);
    }
}

void EncryptionUtilsTest::shouldJoinVectorAndEcryptedData()
{
    QFETCH(EncryptionUtils::EncryptionInfo, encryptionInfo);
    QFETCH(QByteArray, encryptedData);
    QCOMPARE(EncryptionUtils::joinVectorAndEcryptedData(encryptionInfo), encryptedData);
}

void EncryptionUtilsTest::shouldGenerateRandomPassword()
{
    const QString password1 = EncryptionUtils::generateRandomPassword();
    const QString password2 = EncryptionUtils::generateRandomPassword();

    QCOMPARE(password1.size(), 30);
    QCOMPARE(password2.size(), 30);
    QVERIFY(password1 != password2);
}

void EncryptionUtilsTest::shouldGenerateRoomKeyId()
{
    // Must produce a non-empty UUID string.
    const QString keyId1 = EncryptionUtils::generateRoomKeyId();
    QVERIFY(!keyId1.isEmpty());

    // UUID without braces is 36 chars: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    QCOMPARE(keyId1.size(), 36);

    // Must look like a UUID (contains four hyphens at the right positions).
    QCOMPARE(keyId1[8], QChar(u'-'));
    QCOMPARE(keyId1[13], QChar(u'-'));
    QCOMPARE(keyId1[18], QChar(u'-'));
    QCOMPARE(keyId1[23], QChar(u'-'));

    // Each call must produce a unique value.
    const QString keyId2 = EncryptionUtils::generateRoomKeyId();
    QVERIFY(keyId1 != keyId2);
}

#include "moc_encryptionutilstest.cpp"
