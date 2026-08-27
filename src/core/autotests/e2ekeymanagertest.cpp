/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "e2ekeymanagertest.h"
#include "encryption/e2ekeymanager.h"

#include "config-ruqola.h"
#include "encryption/encryptionutils.h"
#include "localdatabase/e2edatabase.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTest>

using namespace Qt::Literals::StringLiterals;

QTEST_GUILESS_MAIN(E2eKeyManagerTest)
E2eKeyManagerTest::E2eKeyManagerTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void E2eKeyManagerTest::shouldHaveDefaultValues()
{
    const E2eKeyManager m(nullptr);
    QCOMPARE(m.status(), E2eKeyManager::Status::Unknown);
    QVERIFY(!m.keySaved());
    QVERIFY(!m.hasPendingUploadFailure());
}

void E2eKeyManagerTest::shouldEmitDecodeSignalOnlyWhenNeeded()
{
    E2eKeyManager manager(nullptr);
    const QSignalSpy spy(&manager, &E2eKeyManager::needDecodeEncryptionKey);

    manager.setStatus(E2eKeyManager::Status::Unknown);
    manager.decodeEncryptionKey();
    QCOMPARE(spy.count(), 0);

    manager.setStatus(E2eKeyManager::Status::NeedToDecryptKey);
    manager.decodeEncryptionKey();
    QCOMPARE(spy.count(), 1);

    manager.setStatus(E2eKeyManager::Status::DecryptionPostponned);
    manager.decodeEncryptionKey();
    QCOMPARE(spy.count(), 2);
}

void E2eKeyManagerTest::shouldSetNeedToDecryptStatusFromBase64StringPayload()
{
    const QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    RocketChatAccount account(tempDir.filePath(u"account.ini"_s));
    account.setAccountName(u"test-e2ekeymanager"_s);
    account.settings()->setUserId("test-e2e-user-string"_ba);

    E2eKeyManager manager(&account);
    const QByteArray encryptedPrivateKey = "encrypted-private-key-data";
    const QByteArray encryptedPrivateKeyBase64 = encryptedPrivateKey.toBase64();

    QJsonObject json;
    json["public_key"_L1] = u"-----BEGIN PUBLIC KEY-----abc-----END PUBLIC KEY-----"_s;
    json["private_key"_L1] = QString::fromLatin1(encryptedPrivateKeyBase64);

    manager.verifyExistingKeyForTest(json);
    QCOMPARE(manager.status(), E2eKeyManager::Status::NeedToDecryptKey);

    QByteArray storedEncryptedPrivateKey;
    QByteArray storedPublicKey;
    QVERIFY(
        account.localDatabaseManager()->e2EDatabase()->loadKey(account.accountName(), u"test-e2e-user-string"_s, storedEncryptedPrivateKey, storedPublicKey));
    QCOMPARE(storedEncryptedPrivateKey, encryptedPrivateKey);
    QCOMPARE(storedPublicKey, json["public_key"_L1].toString().toUtf8());

    QVERIFY(account.localDatabaseManager()->e2EDatabase()->deleteKey(account.accountName(), u"test-e2e-user-string"_s));
}

void E2eKeyManagerTest::shouldSetNeedToDecryptStatusFromBinaryObjectPayload()
{
    const QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    RocketChatAccount account(tempDir.filePath(u"account.ini"_s));
    account.setAccountName(u"test-e2ekeymanager"_s);
    account.settings()->setUserId("test-e2e-user-binary"_ba);

    E2eKeyManager manager(&account);
    const QByteArray encryptedPrivateKey = "binary-private-key-data";

    QJsonObject privateKeyObject;
    privateKeyObject["$binary"_L1] = QString::fromLatin1(encryptedPrivateKey.toBase64());

    QJsonObject json;
    json["public_key"_L1] = u"-----BEGIN PUBLIC KEY-----xyz-----END PUBLIC KEY-----"_s;
    json["private_key"_L1] = privateKeyObject;

    manager.verifyExistingKeyForTest(json);
    QCOMPARE(manager.status(), E2eKeyManager::Status::NeedToDecryptKey);

    QByteArray storedEncryptedPrivateKey;
    QByteArray storedPublicKey;
    QVERIFY(
        account.localDatabaseManager()->e2EDatabase()->loadKey(account.accountName(), u"test-e2e-user-binary"_s, storedEncryptedPrivateKey, storedPublicKey));
    QCOMPARE(storedEncryptedPrivateKey, encryptedPrivateKey);
    QCOMPARE(storedPublicKey, json["public_key"_L1].toString().toUtf8());

    QVERIFY(account.localDatabaseManager()->e2EDatabase()->deleteKey(account.accountName(), u"test-e2e-user-binary"_s));
}

void E2eKeyManagerTest::shouldHandleMissingOrMalformedServerKeys()
{
    const QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    RocketChatAccount account(tempDir.filePath(u"account.ini"_s));
    account.settings()->setAccountName(u"e2e-test-account"_s);
    account.settings()->setServerUrl(u"http://localhost:3000"_s);
    account.settings()->setUserName(u"e2e-test-user"_s);
    account.settings()->setUserId("test-e2e-user-generation"_ba);
    account.settings()->setAuthToken(u"token"_s);

    E2eKeyManager manager(&account);

    {
        const QJsonObject emptyJson;
        manager.verifyExistingKeyForTest(emptyJson);
#if USE_E2E_SUPPORT
        QCOMPARE(manager.status(), E2eKeyManager::Status::NeedToGenerateKey);
#else
        QCOMPARE(manager.status(), E2eKeyManager::Status::Unknown);
#endif
    }

    {
        QJsonObject malformedJson;
        malformedJson["public_key"_L1] = u"present-public-key"_s;
        malformedJson["private_key"_L1] = QJsonObject{};
        manager.verifyExistingKeyForTest(malformedJson);
#if USE_E2E_SUPPORT
        QCOMPARE(manager.status(), E2eKeyManager::Status::NeedToGenerateKey);
#else
        QCOMPARE(manager.status(), E2eKeyManager::Status::Unknown);
#endif
    }

    QVERIFY(account.localDatabaseManager()->e2EDatabase()->deleteKey(account.accountName(), u"test-e2e-user-generation"_s));
}

void E2eKeyManagerTest::shouldKeepGenerationStateAndAllowRetryWhenUploadFails()
{
#if !USE_E2E_SUPPORT
    QSKIP("E2E support is disabled");
#else
    const QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    RocketChatAccount account(tempDir.filePath(u"account.ini"_s));
    account.settings()->setAccountName(u"e2e-test-account"_s);
    account.settings()->setServerUrl(u"http://localhost:3000"_s);
    account.settings()->setUserName(u"e2e-test-user"_s);
    account.settings()->setUserId("test-e2e-user-upload-retry"_ba);
    account.settings()->setAuthToken(u"token"_s);

    E2eKeyManager manager(&account);
    const QSignalSpy uploadFailedSpy(&manager, &E2eKeyManager::uploadEncryptionKeyFailed);

    // Empty server payload triggers local key generation and upload attempt.
    manager.verifyExistingKeyForTest(QJsonObject{});
    QCOMPARE(manager.status(), E2eKeyManager::Status::NeedToGenerateKey);
    QCOMPARE(uploadFailedSpy.count(), 1);
    QVERIFY(manager.hasPendingUploadFailure());

    // Retry should attempt another upload with the same pending generated key data.
    QVERIFY(!manager.retryUploadGeneratedKey());
    QCOMPARE(manager.status(), E2eKeyManager::Status::NeedToGenerateKey);
    QCOMPARE(uploadFailedSpy.count(), 2);
    QVERIFY(manager.hasPendingUploadFailure());

    QVERIFY(account.localDatabaseManager()->e2EDatabase()->deleteKey(account.accountName(), u"test-e2e-user-upload-retry"_s));
#endif
}

void E2eKeyManagerTest::shouldPostponeDecryption()
{
    E2eKeyManager manager(nullptr);
    manager.setStatus(E2eKeyManager::Status::NeedToDecryptKey);

    const QSignalSpy postponedSpy(&manager, &E2eKeyManager::decodeEncryptionKeyPostponed);

    manager.postponeDecryption();

    QCOMPARE(manager.status(), E2eKeyManager::Status::DecryptionPostponned);
    QCOMPARE(postponedSpy.count(), 1);
}

void E2eKeyManagerTest::shouldDecodeEncryptionKeyWithValidPassword()
{
#if !USE_E2E_SUPPORT
    QSKIP("E2E support is disabled");
#else
    const QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    RocketChatAccount account(tempDir.filePath(u"account.ini"_s));
    account.setAccountName(u"test-e2ekeymanager"_s);
    const QString userId = u"test-e2e-user-decode-ok"_s;
    account.settings()->setUserId(userId.toLatin1());

    const QString password = u"my-test-password"_s;
    const auto rsaKeyPair = EncryptionUtils::generateRSAKey();
    QVERIFY(!rsaKeyPair.privateKey.isEmpty());

    const QByteArray masterKey = EncryptionUtils::getMasterKey(password, userId);
    QVERIFY(!masterKey.isEmpty());
    const QByteArray encryptedPrivateKey = EncryptionUtils::encryptPrivateKey(rsaKeyPair.privateKey, masterKey);
    QVERIFY(!encryptedPrivateKey.isEmpty());

    QVERIFY(account.localDatabaseManager()->e2EDatabase()->saveKey(account.accountName(), userId, encryptedPrivateKey, rsaKeyPair.publicKey));

    E2eKeyManager manager(&account);
    manager.setStatus(E2eKeyManager::Status::NeedToDecryptKey);

    const QSignalSpy doneSpy(&manager, &E2eKeyManager::decodeEncryptionKeyDone);
    const QSignalSpy failedSpy(&manager, &E2eKeyManager::failedDecodeEncryptionKey);
    QVERIFY(manager.decodeEncryptionKey(password));
    QCOMPARE(manager.status(), E2eKeyManager::Status::KeyDecrypted);
    QCOMPARE(doneSpy.count(), 1);
    QCOMPARE(failedSpy.count(), 0);

    QVERIFY(account.localDatabaseManager()->e2EDatabase()->deleteKey(account.accountName(), userId));
#endif
}

void E2eKeyManagerTest::shouldFailDecodeEncryptionKeyWithWrongPassword()
{
#if !USE_E2E_SUPPORT
    QSKIP("E2E support is disabled");
#else
    const QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    RocketChatAccount account(tempDir.filePath(u"account.ini"_s));
    account.settings()->setUserId("test-e2e-user-decode-ko"_ba);
    account.setAccountName(u"test-e2ekeymanager"_s);

    const QString userId = u"test-e2e-user-decode-ko"_s;
    const QString password = u"right-password"_s;
    const auto rsaKeyPair = EncryptionUtils::generateRSAKey();
    QVERIFY(!rsaKeyPair.privateKey.isEmpty());

    const QByteArray masterKey = EncryptionUtils::getMasterKey(password, userId);
    QVERIFY(!masterKey.isEmpty());
    const QByteArray encryptedPrivateKey = EncryptionUtils::encryptPrivateKey(rsaKeyPair.privateKey, masterKey);
    QVERIFY(!encryptedPrivateKey.isEmpty());

    QVERIFY(account.localDatabaseManager()->e2EDatabase()->saveKey(account.accountName(), userId, encryptedPrivateKey, rsaKeyPair.publicKey));

    E2eKeyManager manager(&account);
    manager.setStatus(E2eKeyManager::Status::NeedToDecryptKey);

    const QSignalSpy doneSpy(&manager, &E2eKeyManager::decodeEncryptionKeyDone);
    const QSignalSpy failedSpy(&manager, &E2eKeyManager::failedDecodeEncryptionKey);
    QVERIFY(!manager.decodeEncryptionKey(u"wrong-password"_s));
    QCOMPARE(manager.status(), E2eKeyManager::Status::NeedToDecryptKey);
    QCOMPARE(doneSpy.count(), 0);
    QCOMPARE(failedSpy.count(), 1);

    QVERIFY(account.localDatabaseManager()->e2EDatabase()->deleteKey(account.accountName(), userId));
#endif
}

// A password holding a character outside ASCII has to be encoded the way every Rocket.Chat client
// does, one byte per code unit: sealing it the UTF-8 way derives a different master key and leaves
// the private key unusable everywhere else.
void E2eKeyManagerTest::shouldDecodeV2KeySealedWithANonAsciiPassword()
{
#if !USE_E2E_SUPPORT
    QSKIP("E2E support is disabled");
#else
    const QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    RocketChatAccount account(tempDir.filePath(u"account.ini"_s));
    account.setAccountName(u"test-e2ekeymanager"_s);
    const QString userId = u"test-e2e-user-latin1"_s;
    account.settings()->setUserId(userId.toLatin1());

    const QString password = u"p\u00E4ssw\u00F6rd-\u00E9\u00E8"_s;
    const auto rsaKeyPair = EncryptionUtils::generateRSAKey();
    QVERIFY(!rsaKeyPair.privateKey.isEmpty());

    const QByteArray storedKey = EncryptionUtils::encryptPrivateKeyV2(rsaKeyPair.privateKey, password, userId);
    QVERIFY(!storedKey.isEmpty());
    QVERIFY(account.localDatabaseManager()->e2EDatabase()->saveKey(account.accountName(), userId, storedKey, rsaKeyPair.publicKey));

    E2eKeyManager manager(&account);
    manager.setStatus(E2eKeyManager::Status::NeedToDecryptKey);

    const QSignalSpy doneSpy(&manager, &E2eKeyManager::decodeEncryptionKeyDone);
    const QSignalSpy failedSpy(&manager, &E2eKeyManager::failedDecodeEncryptionKey);
    QVERIFY(manager.decodeEncryptionKey(password));
    QCOMPARE(manager.status(), E2eKeyManager::Status::KeyDecrypted);
    QCOMPARE(doneSpy.count(), 1);
    QCOMPARE(failedSpy.count(), 0);

    // The UTF-8 encoding of that password is a different secret and must not open the envelope.
    E2eKeyManager utf8Manager(&account);
    utf8Manager.setStatus(E2eKeyManager::Status::NeedToDecryptKey);
    QVERIFY(!utf8Manager.decodeEncryptionKey(QString::fromLatin1(password.toUtf8())));

    QVERIFY(account.localDatabaseManager()->e2EDatabase()->deleteKey(account.accountName(), userId));
#endif
}

// Keys a previous Ruqola version sealed used the UTF-8 encoding of the password. They have to keep
// opening, otherwise fixing the encoding would lock those accounts out of their own key.
void E2eKeyManagerTest::shouldDecodeV2KeySealedTheOldRuqolaWay()
{
#if !USE_E2E_SUPPORT
    QSKIP("E2E support is disabled");
#else
    const QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    RocketChatAccount account(tempDir.filePath(u"account.ini"_s));
    account.setAccountName(u"test-e2ekeymanager"_s);
    const QString userId = u"test-e2e-user-utf8"_s;
    account.settings()->setUserId(userId.toLatin1());

    const QString password = u"p\u00E4ssw\u00F6rd-\u00E9\u00E8"_s;
    const auto rsaKeyPair = EncryptionUtils::generateRSAKey();
    QVERIFY(!rsaKeyPair.privateKey.isEmpty());

    // Rebuild the V2 envelope by hand, sealed with the master key the old encoding gave.
    constexpr int iterations = 100000;
    const QString salt = u"v2:%1:legacy"_s.arg(userId);
    const QByteArray legacyMasterKey = EncryptionUtils::deriveKey(salt.toUtf8(), password.toUtf8(), iterations, 32);
    QVERIFY(!legacyMasterKey.isEmpty());
    QVERIFY(legacyMasterKey != EncryptionUtils::deriveMasterKey(salt, password, iterations));
    const QByteArray iv = EncryptionUtils::generateRandomIV(12);
    const QByteArray ciphertext = EncryptionUtils::encryptAES_GCM_256(rsaKeyPair.privateKey, legacyMasterKey, iv);
    QVERIFY(!ciphertext.isEmpty());

    QJsonObject storedKey;
    storedKey[QStringLiteral("iv")] = QString::fromLatin1(iv.toBase64());
    storedKey[QStringLiteral("ciphertext")] = QString::fromLatin1(ciphertext.toBase64());
    storedKey[QStringLiteral("salt")] = salt;
    storedKey[QStringLiteral("iterations")] = iterations;
    QVERIFY(account.localDatabaseManager()->e2EDatabase()->saveKey(account.accountName(),
                                                                   userId,
                                                                   QJsonDocument(storedKey).toJson(QJsonDocument::Compact),
                                                                   rsaKeyPair.publicKey));

    E2eKeyManager manager(&account);
    manager.setStatus(E2eKeyManager::Status::NeedToDecryptKey);

    const QSignalSpy doneSpy(&manager, &E2eKeyManager::decodeEncryptionKeyDone);
    const QSignalSpy failedSpy(&manager, &E2eKeyManager::failedDecodeEncryptionKey);
    QVERIFY(manager.decodeEncryptionKey(password));
    QCOMPARE(manager.status(), E2eKeyManager::Status::KeyDecrypted);
    QCOMPARE(doneSpy.count(), 1);
    QCOMPARE(failedSpy.count(), 0);

    // A genuinely wrong password must still be refused, whichever encoding is tried.
    E2eKeyManager wrongManager(&account);
    wrongManager.setStatus(E2eKeyManager::Status::NeedToDecryptKey);
    QVERIFY(!wrongManager.decodeEncryptionKey(u"wrong-p\u00E4ssword"_s));

    QVERIFY(account.localDatabaseManager()->e2EDatabase()->deleteKey(account.accountName(), userId));
#endif
}

#include "moc_e2ekeymanagertest.cpp"
