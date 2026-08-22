/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "encryptionutilstest.h"
#include "encryption/encryptionutils.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
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
    QCOMPARE(publicKeyObject.value(QStringLiteral("kty")).toString(), QStringLiteral("RSA"));
    QVERIFY(!publicKeyObject.value(QStringLiteral("n")).toString().isEmpty());

    // We store that JWK locally and encrypt the room keys with it, so it must convert back.
    RSA *publicKey = EncryptionUtils::publicKeyFromPEM(EncryptionUtils::publicKeyJWKToPEM(publicKeyJwk));
    QVERIFY(publicKey);
    RSA_free(publicKey);

    const QJsonObject privateKeyObject = QJsonDocument::fromJson(privateKeyJwk).object();
    QCOMPARE(privateKeyObject.value(QStringLiteral("kty")).toString(), QStringLiteral("RSA"));
    for (const QString &component : {QStringLiteral("n"),
                                     QStringLiteral("e"),
                                     QStringLiteral("d"),
                                     QStringLiteral("p"),
                                     QStringLiteral("q"),
                                     QStringLiteral("dp"),
                                     QStringLiteral("dq"),
                                     QStringLiteral("qi")}) {
        QVERIFY2(!privateKeyObject.value(component).toString().isEmpty(), qPrintable(component));
    }
    // The JWK must describe the very same key.
    QCOMPARE(EncryptionUtils::privateKeyJWKToPEM(privateKeyJwk), keyPair.privateKey);

    const QString password = QStringLiteral("secret password");
    const QString userId = QStringLiteral("userId");
    const QByteArray storedKey = EncryptionUtils::encryptPrivateKeyV2(privateKeyJwk, password, userId);
    const QJsonObject storedKeyObject = QJsonDocument::fromJson(storedKey).object();
    QVERIFY(storedKeyObject.value(QStringLiteral("salt")).toString().startsWith(QStringLiteral("v2:") + userId + QLatin1Char(':')));
    QCOMPARE(storedKeyObject.value(QStringLiteral("iterations")).toInt(), 100000);

    // Decrypting it the way every client does must give the JWK private key back. A 16-byte IV
    // would be understood as the legacy AES-CBC layout, so it has to be shorter.
    const QByteArray iv = QByteArray::fromBase64(storedKeyObject.value(QStringLiteral("iv")).toString().toUtf8());
    QCOMPARE(iv.size(), 12);
    const QByteArray masterKey = EncryptionUtils::deriveKey(storedKeyObject.value(QStringLiteral("salt")).toString().toUtf8(),
                                                            password.toUtf8(),
                                                            storedKeyObject.value(QStringLiteral("iterations")).toInt(),
                                                            32);
    const QByteArray ciphertext = QByteArray::fromBase64(storedKeyObject.value(QStringLiteral("ciphertext")).toString().toUtf8());
    QCOMPARE(EncryptionUtils::decryptAES_GCM_256(ciphertext, masterKey, iv), privateKeyJwk);

    // A wrong password must not decrypt it.
    const QByteArray wrongMasterKey = EncryptionUtils::deriveKey(storedKeyObject.value(QStringLiteral("salt")).toString().toUtf8(),
                                                                 QByteArrayLiteral("other password"),
                                                                 storedKeyObject.value(QStringLiteral("iterations")).toInt(),
                                                                 32);
    QVERIFY(EncryptionUtils::decryptAES_GCM_256(ciphertext, wrongMasterKey, iv).isEmpty());
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
        QTest::addRow("too-short") << QByteArray("1234567890123456") << info;
    }
    {
        EncryptionUtils::EncryptionInfo info;
        info.vector = QByteArray("1234567890abcdef");
        info.encryptedData = QByteArray("cipher-payload");
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
        info.encryptedData = "blafoo-z";
        info.vector = "AAAPPLLLAPPPAPAPPAPA";
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
