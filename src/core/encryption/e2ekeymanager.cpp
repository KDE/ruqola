/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ekeymanager.h"
#include "config-ruqola.h"
#include "connection.h"
#include "e2e/fetchmykeysjob.h"
#include "e2e/setuserpublicandprivatekeysjob.h"
#if USE_E2E_SUPPORT
#include "encryptionutils.h"
#endif
#include "localdatabase/e2edatabase.h"
#include "localdatabase/localdatabasemanager.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include "ruqola_encryption_debug.h"
#include "ruqolaserverconfig.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

using namespace Qt::Literals::StringLiterals;

// https://docs.rocket.chat/docs/end-to-end-encryption-specifications
E2eKeyManager::E2eKeyManager(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mAccount(account)
{
}

E2eKeyManager::~E2eKeyManager() = default;

void E2eKeyManager::decodeEncryptionKey()
{
    if (mStatus == Status::NeedToDecryptKey || mStatus == Status::DecryptionPostponned) {
        Q_EMIT needDecodeEncryptionKey();
    }
}

bool E2eKeyManager::decodeEncryptionKey(const QString &password)
{
#if USE_E2E_SUPPORT
    if (!mAccount || password.isEmpty()) {
        setStatus(Status::NeedToDecryptKey);
        Q_EMIT failedDecodeEncryptionKey();
        return false;
    }

    const QString userId = QString::fromLatin1(mAccount->settings()->userId());
    if (userId.isEmpty()) {
        setStatus(Status::NeedToDecryptKey);
        Q_EMIT failedDecodeEncryptionKey();
        return false;
    }

    QByteArray encryptedPrivateKey;
    QByteArray publicKey;
    if (!mAccount->localDatabaseManager()->e2EDatabase()->loadKey(userId, encryptedPrivateKey, publicKey)) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to decode E2E key: no local encrypted private key found";
        setStatus(Status::NeedToDecryptKey);
        Q_EMIT failedDecodeEncryptionKey();
        return false;
    }

    // Decrypt the stored private key.  Two storage layouts are possible:
    //
    //  V2 JSON  – starts with '{'; contains its own PBKDF2 salt/iterations
    //             and was encrypted with AES-GCM.
    //  Binary   – raw bytes: iv[16] + AES-CBC-256 ciphertext; PBKDF2 uses
    //             the userId as salt with 1 000 iterations.
    //
    // After decryption, the plaintext may be:
    //   • JWK JSON  (starts with '{') – produced by Rocket.Chat web/mobile
    //   • PEM       – produced by Ruqola itself
    QByteArray decryptedPrivateKey;
    if (encryptedPrivateKey.startsWith('{')) {
        // ── V2 format (AES-GCM) ─────────────────────────────────────────────
        const QJsonDocument doc = QJsonDocument::fromJson(encryptedPrivateKey);
        if (doc.isNull() || !doc.isObject()) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to parse V2 encrypted private key JSON";
            setStatus(Status::NeedToDecryptKey);
            Q_EMIT failedDecodeEncryptionKey();
            return false;
        }
        const QJsonObject v2 = doc.object();
        const QString v2Salt = v2.value(QStringLiteral("salt")).toString();
        const int v2Iterations = v2.value(QStringLiteral("iterations")).toInt();
        const QByteArray v2Iv = QByteArray::fromBase64(v2.value(QStringLiteral("iv")).toString().toUtf8());
        const QByteArray v2Ciphertext = QByteArray::fromBase64(v2.value(QStringLiteral("ciphertext")).toString().toUtf8());

        if (v2Salt.isEmpty() || v2Iterations <= 0 || v2Iv.isEmpty() || v2Ciphertext.isEmpty()) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "V2 encrypted private key has missing fields";
            setStatus(Status::NeedToDecryptKey);
            Q_EMIT failedDecodeEncryptionKey();
            return false;
        }

        const QByteArray v2MasterKey = EncryptionUtils::deriveKey(v2Salt.toUtf8(), password.toUtf8(), v2Iterations, 32);
        if (v2MasterKey.isEmpty()) {
            setStatus(Status::NeedToDecryptKey);
            Q_EMIT failedDecodeEncryptionKey();
            return false;
        }

        decryptedPrivateKey = EncryptionUtils::decryptAES_GCM_256(v2Ciphertext, v2MasterKey, v2Iv);
    } else {
        // ── V1 / oldest format (AES-CBC) ────────────────────────────────────
        const QByteArray masterKey = EncryptionUtils::getMasterKey(password, userId);
        if (masterKey.isEmpty()) {
            setStatus(Status::NeedToDecryptKey);
            Q_EMIT failedDecodeEncryptionKey();
            return false;
        }
        decryptedPrivateKey = EncryptionUtils::decryptPrivateKey(encryptedPrivateKey, masterKey);
    }

    if (decryptedPrivateKey.isEmpty()) {
        setStatus(Status::NeedToDecryptKey);
        Q_EMIT failedDecodeEncryptionKey();
        return false;
    }

    // Convert decrypted bytes to PEM.
    // Rocket.Chat web/mobile encrypts the private key serialised as JWK JSON;
    // Ruqola-generated keys are already PEM.
    QByteArray privateKeyPem;
    if (decryptedPrivateKey.startsWith('{')) {
        privateKeyPem = EncryptionUtils::privateKeyJWKToPEM(decryptedPrivateKey);
    } else {
        privateKeyPem = decryptedPrivateKey;
    }

    if (privateKeyPem.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to obtain PEM from decrypted private key";
        setStatus(Status::NeedToDecryptKey);
        Q_EMIT failedDecodeEncryptionKey();
        return false;
    }

    RSA *privateKey = EncryptionUtils::privateKeyFromPEM(privateKeyPem);
    if (!privateKey) {
        setStatus(Status::NeedToDecryptKey);
        Q_EMIT failedDecodeEncryptionKey();
        return false;
    }

    RSA_free(privateKey);
    mDecodedPrivateKey = privateKeyPem;
    setStatus(Status::KeyDecrypted);
    Q_EMIT decodeEncryptionKeyDone();
    return true;
#else
    Q_UNUSED(password)
    return false;
#endif
}

void E2eKeyManager::postponeDecryption()
{
    setStatus(Status::DecryptionPostponned);
    Q_EMIT decodeEncryptionKeyPostponed();
}

bool E2eKeyManager::retryUploadGeneratedKey()
{
#if USE_E2E_SUPPORT
    if (!mAccount || !mPendingUploadFailed || mPendingUploadPublicKey.isEmpty() || mPendingUploadPrivateKey.isEmpty()) {
        return false;
    }

    setStatus(Status::NeedToGenerateKey);
    return startUploadGeneratedKey(mPendingUploadPublicKey, mPendingUploadPrivateKey);
#else
    return false;
#endif
}

bool E2eKeyManager::hasPendingUploadFailure() const
{
    return mPendingUploadFailed;
}

QString E2eKeyManager::generateRandomPassword() const
{
#if USE_E2E_SUPPORT
    if (!mGeneratedPassword.isEmpty()) {
        return mGeneratedPassword;
    }
    return EncryptionUtils::generateRandomPassword();
#else
    return {};
#endif
}

E2eKeyManager::Status E2eKeyManager::status() const
{
    return mStatus;
}

void E2eKeyManager::setStatus(Status newStatus)
{
    mStatus = newStatus;
}

void E2eKeyManager::fetchMyKeys()
{
    auto job = new RocketChatRestApi::FetchMyKeysJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::FetchMyKeysJob::fetchMyKeysDone, this, [this](const QJsonObject &json) {
        qDebug() << " RocketChatRestApi::FetchMyKeysJob result " << json << "account name: " << mAccount->accountName();
        verifyExistingKey(json);
        Q_EMIT verifyKeyDone();
    });
    if (!job->start()) {
        qCDebug(RUQOLA_ENCRYPTION_LOG) << "Impossible to start fetchmykeys job";
    }
}

void E2eKeyManager::verifyExistingKey(const QJsonObject &json)
{
    // Decode the server's private_key field into the bytes we store locally.
    //
    // The field can arrive in several formats depending on the server version:
    //
    //  Oldest : plain base64 string  →  binary (iv[16] + AES-CBC ciphertext)
    //  V1     : JSON object { "$binary": "<base64>" }  →  same binary layout
    //  V2     : JSON object { "iv":"…", "ciphertext":"…", "salt":"…",
    //                         "iterations": N }  →  stored as compact JSON bytes
    //
    // We also handle the unusual case where the server serialises V1/V2 as a
    // JSON *string* (i.e. the value is already JSON-stringified).
    const auto decodeEncryptedPrivateKey = [](const QJsonValue &privateKeyValue) -> QByteArray {
        // Helper: process a QJsonObject for V1 ($binary) or V2 (iv/ciphertext)
        const auto decodeObject = [](const QJsonObject &obj) -> QByteArray {
            // V1: {"$binary": "<base64>"}
            const QString binaryValue = obj.value(QStringLiteral("$binary")).toString();
            if (!binaryValue.isEmpty()) {
                return QByteArray::fromBase64(binaryValue.toUtf8());
            }
            // V2: {"iv":…, "ciphertext":…, "salt":…, "iterations":…}
            if (obj.contains(QStringLiteral("iv")) && obj.contains(QStringLiteral("ciphertext")) && obj.contains(QStringLiteral("salt"))) {
                return QJsonDocument(obj).toJson(QJsonDocument::Compact);
            }
            return {};
        };

        if (privateKeyValue.isObject()) {
            return decodeObject(privateKeyValue.toObject());
        }

        if (privateKeyValue.isString()) {
            const QString str = privateKeyValue.toString();
            const QByteArray strBytes = str.toUtf8();

            // Check whether the string is itself a JSON object (server stringified it)
            if (str.startsWith(QLatin1Char('{'))) {
                const QJsonDocument doc = QJsonDocument::fromJson(strBytes);
                if (!doc.isNull() && doc.isObject()) {
                    const QByteArray result = decodeObject(doc.object());
                    if (!result.isEmpty()) {
                        return result;
                    }
                }
            }

            // Oldest format: plain base64 string → binary (iv + ciphertext)
            const QByteArray decoded = QByteArray::fromBase64(strBytes);
            return decoded.isEmpty() ? strBytes : decoded;
        }

        return {};
    };

    if (!mAccount) {
        setStatus(Status::Unknown);
        return;
    }

    const QString publicKey = json.value("public_key"_L1).toString();
    const QByteArray encryptedPrivateKey = decodeEncryptedPrivateKey(json.value("private_key"_L1));

    if (!publicKey.isEmpty() && !encryptedPrivateKey.isEmpty()) {
        const QString userId = QString::fromLatin1(mAccount->settings()->userId());
        if (!userId.isEmpty()) {
            (void)mAccount->localDatabaseManager()->e2EDatabase()->saveKey(userId, encryptedPrivateKey, publicKey.toUtf8());
        }
        setStatus(Status::NeedToDecryptKey);
        return;
    }

#if USE_E2E_SUPPORT
    const QString userId = QString::fromLatin1(mAccount->settings()->userId());
    if (userId.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to generate E2E keys: user id is empty";
        setStatus(Status::Unknown);
        return;
    }

    mGeneratedPassword = EncryptionUtils::generateRandomPassword();
    if (mGeneratedPassword.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to generate E2E keys: random password generation failed";
        setStatus(Status::Unknown);
        return;
    }

    const QByteArray masterKey = EncryptionUtils::getMasterKey(mGeneratedPassword, userId);
    const EncryptionUtils::RSAKeyPair rsaKeyPair = EncryptionUtils::generateRSAKey();
    if (masterKey.isEmpty() || rsaKeyPair.privateKey.isEmpty() || rsaKeyPair.publicKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to generate E2E keys: prerequisite generation failed";
        setStatus(Status::Unknown);
        return;
    }

    const QByteArray encryptedGeneratedPrivateKey = EncryptionUtils::encryptPrivateKey(rsaKeyPair.privateKey, masterKey);
    if (encryptedGeneratedPrivateKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to generate E2E keys: private key encryption failed";
        setStatus(Status::Unknown);
        return;
    }

    (void)mAccount->localDatabaseManager()->e2EDatabase()->saveKey(userId, encryptedGeneratedPrivateKey, rsaKeyPair.publicKey);

    // Local key material is ready at this point, so keep generation state even if upload cannot start.
    setStatus(Status::NeedToGenerateKey);
    startUploadGeneratedKey(rsaKeyPair.publicKey, encryptedGeneratedPrivateKey);
#else
    setStatus(Status::Unknown);
#endif
}

bool E2eKeyManager::startUploadGeneratedKey(const QByteArray &publicKey, const QByteArray &encryptedPrivateKey)
{
    if (!mAccount || publicKey.isEmpty() || encryptedPrivateKey.isEmpty()) {
        return false;
    }

    mPendingUploadPublicKey = publicKey;
    mPendingUploadPrivateKey = encryptedPrivateKey;
    mPendingUploadFailed = false;

    auto setJob = new RocketChatRestApi::SetUserPublicAndPrivateKeysJob(this);
    mAccount->restApi()->initializeRestApiJob(setJob);

    RocketChatRestApi::SetUserPublicAndPrivateKeysJob::SetUserPublicAndPrivateKeysInfo info;
    info.rsaPublicKey = QString::fromUtf8(publicKey);
    info.rsaPrivateKey = QString::fromLatin1(encryptedPrivateKey.toBase64());
    setJob->setSetUserPublicAndPrivateKeysInfo(info);

    connect(setJob, &RocketChatRestApi::SetUserPublicAndPrivateKeysJob::setUserPublicAndPrivateKeysDone, this, [this]() {
        mPendingUploadFailed = false;
        Q_EMIT uploadEncryptionKeyDone();
    });
    connect(setJob, &RocketChatRestApi::RestApiAbstractJob::failed, this, [this](const QString &, const QString &) {
        mPendingUploadFailed = true;
        setStatus(Status::NeedToGenerateKey);
        Q_EMIT uploadEncryptionKeyFailed();
    });

    if (!setJob->start()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to upload generated E2E keypair";
        mPendingUploadFailed = true;
        setStatus(Status::NeedToGenerateKey);
        Q_EMIT uploadEncryptionKeyFailed();
        return false;
    }

    return true;
}

void E2eKeyManager::verifyExistingKeyForTest(const QJsonObject &json)
{
    verifyExistingKey(json);
}

bool E2eKeyManager::keySaved() const
{
    if (mAccount) {
        return mAccount->settings()->keySaved();
    }
    return false;
}

void E2eKeyManager::setKeySaved(bool newKeySaved)
{
    if (mAccount) {
        mAccount->settings()->setKeySaved(newKeySaved);
    }
}

E2eKeyManager::Status E2eKeyManager::needToDecodeEncryptionKey() const
{
    if (!mAccount) {
        return Status::Unknown;
    }
    if (mAccount->ruqolaServerConfig()->encryptionEnabled()) {
        return mStatus;
    }
    return Status::Unknown;
}

#include "moc_e2ekeymanager.cpp"
