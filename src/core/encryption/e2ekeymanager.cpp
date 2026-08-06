/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ekeymanager.h"
#include "config-ruqola.h"
#include "connection.h"
#include "e2e/fetchmykeysjob.h"
#include "e2e/getusersofroomwithoutkeyjob.h"
#include "e2e/provideuserswithsuggestedgroupkeysjob.h"
#include "e2e/setroomkeyidjob.h"
#include "e2e/setuserpublicandprivatekeysjob.h"
#include "e2e/updategroupkeyjob.h"
#if USE_E2E_SUPPORT
#include "encryptionutils.h"
#endif
#include "localdatabase/e2edatabase.h"
#include "localdatabase/e2eroomsdatabase.h"
#include "localdatabase/localdatabasemanager.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include "room.h"
#include "ruqola_encryption_debug.h"
#include "ruqolaserverconfig.h"
#include <qt6keychain/keychain.h>

#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTimer>
using namespace QKeychain;
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
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to decode E2E key: no userId found";
        Q_EMIT failedDecodeEncryptionKey();
        return false;
    }

    QByteArray encryptedPrivateKey;
    QByteArray publicKey;
    if (!mAccount->localDatabaseManager()->e2EDatabase()->loadKey(mAccount->accountName(), userId, encryptedPrivateKey, publicKey)) {
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
    if (decryptRoomsSessionKeys()) {
        Q_EMIT needRefreshView();
    }
    Q_EMIT decodeEncryptionKeyDone();
    storePassword(password);
    return true;
#else
    Q_UNUSED(password)
    return false;
#endif
}

QString E2eKeyManager::passwordKeyIdentifier() const
{
    return mAccount->accountName() + u"-encrypted"_s;
}

void E2eKeyManager::storePassword(const QString &password)
{
    auto writeJob = new WritePasswordJob(u"Ruqola"_s);
    connect(writeJob, &Job::finished, this, &E2eKeyManager::slotPasswordWritten);
    writeJob->setKey(passwordKeyIdentifier());
    writeJob->setTextData(password);
    writeJob->start();
}

void E2eKeyManager::slotPasswordWritten(QKeychain::Job *baseJob)
{
    if (baseJob->error()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error writing password using QKeychain:" << baseJob->errorString();
    }
}

void E2eKeyManager::readPassword()
{
    auto readJob = new ReadPasswordJob(u"Ruqola"_s);
    connect(readJob, &Job::finished, this, &E2eKeyManager::slotPasswordRead);
    readJob->setKey(passwordKeyIdentifier());
    readJob->start();
}

void E2eKeyManager::slotPasswordRead(QKeychain::Job *baseJob)
{
    auto job = qobject_cast<ReadPasswordJob *>(baseJob);
    Q_ASSERT(job);
    if (!job->error()) {
        const QString password = job->textData();
        qCDebug(RUQOLA_ENCRYPTION_LOG) << "OK, we have the password now";
        if (!decodeEncryptionKey(password)) {
            qCDebug(RUQOLA_ENCRYPTION_LOG) << "Impossible to decode encryption key";
        }
    } else {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "We have an error during reading password " << job->errorString() << " Account name " << mAccount->accountName();
    }
    Q_EMIT verifyKeyDone();
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

bool E2eKeyManager::initializeRoomE2EKey([[maybe_unused]] const QByteArray &roomId, [[maybe_unused]] const QString &existingKeyId)
{
#if USE_E2E_SUPPORT
    if (!mAccount || roomId.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: invalid arguments";
        return false;
    }
    if (mStatus != Status::KeyDecrypted || mDecodedPrivateKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: E2E key not yet decrypted";
        return false;
    }

    // Generate a new 32-byte AES-256-GCM session key.
    const QByteArray sessionKey = EncryptionUtils::generateSessionKey();

    if (existingKeyId.isEmpty()) {
        // Server has no keyId yet — register a newly generated one.
        const QString keyId = EncryptionUtils::generateRoomKeyId();
        auto setKeyIdJob = new RocketChatRestApi::SetRoomKeyIDJob(this);
        mAccount->restApi()->initializeRestApiJob(setKeyIdJob);
        const RocketChatRestApi::SetRoomKeyIDJob::RoomKeyIDInfo keyIdInfo{
            .roomId = roomId,
            .keyId = keyId.toLatin1(),
        };
        setKeyIdJob->setRoomKeyIDInfo(keyIdInfo);
        connect(setKeyIdJob, &RocketChatRestApi::SetRoomKeyIDJob::setRoomKeyIdDone, this, [sessionKey, keyId, roomId, this]() {
            distributeRoomSessionKey(roomId, sessionKey, keyId);
        });
        connect(setKeyIdJob, &RocketChatRestApi::SetRoomKeyIDJob::roomKeyIdAlreadyExists, this, [sessionKey, roomId, this]() {
            const auto tryDistributeUsingServerKeyId = [this, roomId, sessionKey](int delayMs) {
                QTimer::singleShot(delayMs, this, [this, roomId, sessionKey]() {
                    Room *const room = mAccount->room(roomId);
                    if (!room) {
                        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: unable to recover from key-id race, room not found" << roomId;
                        return;
                    }

                    const QString serverKeyId = room->e2eKeyId();
                    if (serverKeyId.isEmpty()) {
                        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: waiting for server key-id update after race" << roomId;
                        return;
                    }

                    qCDebug(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: recovered room key-id after race" << serverKeyId << "for" << roomId;
                    distributeRoomSessionKey(roomId, sessionKey, serverKeyId);
                });
            };

            // Room updates are asynchronous; try shortly after the conflict to reuse server key id.
            tryDistributeUsingServerKeyId(250);
            tryDistributeUsingServerKeyId(1000);
        });
        if (!setKeyIdJob->start()) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: failed to start SetRoomKeyIDJob";
            return false;
        }
    } else {
        // Server already assigned a keyId (e.g. auto-created by the server on room creation).
        qCDebug(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: reusing server-assigned keyId" << existingKeyId;
        distributeRoomSessionKey(roomId, sessionKey, existingKeyId);
    }
    return true;
#else
    return false;
#endif
}

bool E2eKeyManager::distributeExistingRoomE2EKey(const QByteArray &roomId)
{
#if USE_E2E_SUPPORT
    if (!mAccount || roomId.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "distributeExistingRoomE2EKey: invalid arguments";
        return false;
    }

    if (mStatus != Status::KeyDecrypted || mDecodedPrivateKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "distributeExistingRoomE2EKey: E2E key not yet decrypted";
        return false;
    }

    Room *const room = mAccount->room(roomId);
    if (!room || !room->encrypted()) {
        return false;
    }

    const QByteArray sessionKey = room->sessionKey();
    if (sessionKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "distributeExistingRoomE2EKey: missing room session key for" << roomId;
        return false;
    }

    const QString keyId = room->e2eKeyId();
    if (keyId.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "distributeExistingRoomE2EKey: missing room key id for" << roomId;
        return false;
    }

    distributeRoomSessionKey(roomId, sessionKey, keyId);
    return true;
#else
    Q_UNUSED(roomId)
    return false;
#endif
}

void E2eKeyManager::distributeRoomSessionKey([[maybe_unused]] const QByteArray &roomId,
                                             [[maybe_unused]] const QByteArray &sessionKey,
                                             [[maybe_unused]] const QString &keyId)
{
#if USE_E2E_SUPPORT
    // Retrieve own RSA public key from local database.
    const QString userId = QString::fromLatin1(mAccount->settings()->userId());
    QByteArray encryptedOwnPrivateKey;
    QByteArray ownPublicKeyPem;
    if (!mAccount->localDatabaseManager()->e2EDatabase()->loadKey(mAccount->accountName(), userId, encryptedOwnPrivateKey, ownPublicKeyPem)) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: own public key not found in local database";
        return;
    }

    // Encrypt the session key with own RSA-OAEP public key.
    // The public key may be stored as JWK JSON (Rocket.Chat format) or PEM.
    QByteArray resolvedPublicKeyPem;
    if (ownPublicKeyPem.trimmed().startsWith('{')) {
        resolvedPublicKeyPem = EncryptionUtils::publicKeyJWKToPEM(ownPublicKeyPem);
        if (resolvedPublicKeyPem.isEmpty()) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: failed to convert JWK public key to PEM";
            return;
        }
    } else {
        resolvedPublicKeyPem = ownPublicKeyPem;
    }
    RSA *rsaPublicKey = EncryptionUtils::publicKeyFromPEM(resolvedPublicKeyPem);
    if (!rsaPublicKey) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: failed to parse own public key";
        return;
    }
    // Rocket.Chat expects the RSA-encrypted payload to be the JWK JSON bytes of
    // the session key (not raw bytes).  Wrap before encrypting.
    const QByteArray sessionKeyJwk = EncryptionUtils::sessionKeyToJWK(sessionKey);
    if (sessionKeyJwk.isEmpty()) {
        RSA_free(rsaPublicKey);
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: failed to encode session key as JWK";
        return;
    }
    const QByteArray encryptedSessionKey = EncryptionUtils::encryptSessionKey(sessionKeyJwk, rsaPublicKey);
    RSA_free(rsaPublicKey);
    if (encryptedSessionKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: session key encryption failed";
        return;
    }

    // Prime the local room state immediately so the sender can encrypt outgoing
    // messages without waiting for asynchronous server subscription updates.
    if (Room *const room = mAccount->room(roomId)) {
        room->setE2eKeyId(keyId);
        room->setE2EKey(keyId + QString::fromLatin1(encryptedSessionKey.toBase64()));
        if (!decryptRoomSessionKeys(room)) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: unable to decrypt local room session key immediately for" << roomId;
        }
    }

    // Store the encrypted session key in the user's subscription on the server.
    auto updateKeyJob = new RocketChatRestApi::UpdateGroupKeyJob(this);
    mAccount->restApi()->initializeRestApiJob(updateKeyJob);
    RocketChatRestApi::UpdateGroupKeyJob::UpdateGroupKeyInfo updateInfo;
    updateInfo.uid = userId;
    updateInfo.roomId = QString::fromLatin1(roomId);
    updateInfo.key = QString::fromLatin1(encryptedSessionKey.toBase64());
    updateKeyJob->setUpdateGroupInfo(updateInfo);
    if (!updateKeyJob->start()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: failed to start UpdateGroupKeyJob";
        return;
    }

    // Share this room key with users that do not have a key yet.
    auto usersWithoutKeyJob = new RocketChatRestApi::GetUsersOfRoomWithoutKeyJob(this);
    mAccount->restApi()->initializeRestApiJob(usersWithoutKeyJob);
    usersWithoutKeyJob->setRoomId(roomId);
    connect(usersWithoutKeyJob,
            &RocketChatRestApi::GetUsersOfRoomWithoutKeyJob::getUsersOfRoomWithoutKeyDone,
            this,
            [this, roomId, sessionKey, keyId](const QJsonObject &obj) {
                const QJsonArray users = obj.value("users"_L1).toArray();
                if (users.isEmpty()) {
                    return;
                }

                const QString ownUserId = QString::fromLatin1(mAccount->settings()->userId());
                QVector<RocketChatRestApi::SuggestedGroupKey> suggestedKeys;
                suggestedKeys.reserve(users.size());

                for (const QJsonValue &userValue : users) {
                    const QJsonObject userObj = userValue.toObject();
                    const QString targetUserId = userObj.value("_id"_L1).toString();
                    if (targetUserId.isEmpty() || targetUserId == ownUserId) {
                        continue;
                    }

                    const QString publicKey = userObj.value("e2e"_L1).toObject().value("public_key"_L1).toString();
                    if (publicKey.isEmpty()) {
                        continue;
                    }

                    QByteArray publicKeyPem = publicKey.toUtf8();
                    if (publicKeyPem.trimmed().startsWith('{')) {
                        publicKeyPem = EncryptionUtils::publicKeyJWKToPEM(publicKeyPem);
                    }
                    if (publicKeyPem.isEmpty()) {
                        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: unable to resolve recipient public key for" << targetUserId;
                        continue;
                    }

                    RSA *targetRsaPublicKey = EncryptionUtils::publicKeyFromPEM(publicKeyPem);
                    if (!targetRsaPublicKey) {
                        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: unable to parse recipient public key for" << targetUserId;
                        continue;
                    }

                    // Encode session key as JWK before RSA-encrypting (Rocket.Chat format).
                    const QByteArray recipientSessionKeyJwk = EncryptionUtils::sessionKeyToJWK(sessionKey);
                    if (recipientSessionKeyJwk.isEmpty()) {
                        RSA_free(targetRsaPublicKey);
                        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: unable to encode session key as JWK for" << targetUserId;
                        continue;
                    }
                    const QByteArray encryptedRecipientSessionKey = EncryptionUtils::encryptSessionKey(recipientSessionKeyJwk, targetRsaPublicKey);
                    RSA_free(targetRsaPublicKey);
                    if (encryptedRecipientSessionKey.isEmpty()) {
                        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: unable to encrypt room key for" << targetUserId;
                        continue;
                    }

                    suggestedKeys.append({
                        targetUserId,
                        keyId + QString::fromLatin1(encryptedRecipientSessionKey.toBase64()),
                    });
                }

                if (suggestedKeys.isEmpty()) {
                    return;
                }

                auto provideJob = new RocketChatRestApi::ProvideUsersWithSuggestedGroupKeysJob(this);
                mAccount->restApi()->initializeRestApiJob(provideJob);
                provideJob->setRoomId(QString::fromLatin1(roomId));
                provideJob->setKeys(suggestedKeys);
                if (!provideJob->start()) {
                    qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: failed to start ProvideUsersWithSuggestedGroupKeysJob for room" << roomId;
                }
            });

    if (!usersWithoutKeyJob->start()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: failed to start GetUsersOfRoomWithoutKeyJob for room" << roomId;
    }

    // Persist the encrypted session key locally so we can decrypt messages without a server round-trip.
    (void)mAccount->localDatabaseManager()->e2ERoomsDataBase()->saveKey(mAccount->accountName(),
                                                                        QString::fromLatin1(roomId),
                                                                        keyId,
                                                                        encryptedSessionKey,
                                                                        ownPublicKeyPem);

    qCDebug(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: E2E session key initialised for room" << roomId << "keyId" << keyId;
#endif
}

void E2eKeyManager::fetchMyKeys()
{
    auto job = new RocketChatRestApi::FetchMyKeysJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::FetchMyKeysJob::fetchMyKeysDone, this, [this](const QJsonObject &json) {
        verifyExistingKey(json);
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
        Q_EMIT verifyKeyDone();
        return;
    }

    const QString publicKey = json.value("public_key"_L1).toString();
    const QByteArray encryptedPrivateKey = decodeEncryptedPrivateKey(json.value("private_key"_L1));

    if (!publicKey.isEmpty() && !encryptedPrivateKey.isEmpty()) {
        const QString userId = QString::fromLatin1(mAccount->settings()->userId());
        if (!userId.isEmpty()) {
            (void)mAccount->localDatabaseManager()->e2EDatabase()->saveKey(mAccount->accountName(), userId, encryptedPrivateKey, publicKey.toUtf8());
        }
        readPassword();
        setStatus(Status::NeedToDecryptKey);
        return;
    }

#if USE_E2E_SUPPORT
    const QString userId = QString::fromLatin1(mAccount->settings()->userId());
    if (userId.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to generate E2E keys: user id is empty";
        setStatus(Status::Unknown);
        Q_EMIT verifyKeyDone();
        return;
    }

    mGeneratedPassword = EncryptionUtils::generateRandomPassword();
    if (mGeneratedPassword.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to generate E2E keys: random password generation failed";
        setStatus(Status::Unknown);
        Q_EMIT verifyKeyDone();
        return;
    }

    const QByteArray masterKey = EncryptionUtils::getMasterKey(mGeneratedPassword, userId);
    const EncryptionUtils::RSAKeyPair rsaKeyPair = EncryptionUtils::generateRSAKey();
    if (masterKey.isEmpty() || rsaKeyPair.privateKey.isEmpty() || rsaKeyPair.publicKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to generate E2E keys: prerequisite generation failed";
        setStatus(Status::Unknown);
        Q_EMIT verifyKeyDone();
        return;
    }

    const QByteArray encryptedGeneratedPrivateKey = EncryptionUtils::encryptPrivateKey(rsaKeyPair.privateKey, masterKey);
    if (encryptedGeneratedPrivateKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to generate E2E keys: private key encryption failed";
        setStatus(Status::Unknown);
        Q_EMIT verifyKeyDone();
        return;
    }

    (void)mAccount->localDatabaseManager()->e2EDatabase()->saveKey(mAccount->accountName(), userId, encryptedGeneratedPrivateKey, rsaKeyPair.publicKey);

    qCDebug(RUQOLA_ENCRYPTION_LOG) << "rsaKeyPair.publicKey" << rsaKeyPair.publicKey << " encryptedGeneratedPrivateKey " << encryptedGeneratedPrivateKey;
    // Local key material is ready at this point, so keep generation state even if upload cannot start.
    setStatus(Status::NeedToGenerateKey);
    startUploadGeneratedKey(rsaKeyPair.publicKey, encryptedGeneratedPrivateKey);
    Q_EMIT verifyKeyDone();
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

bool E2eKeyManager::decryptRoomsSessionKeys() const
{
#if USE_E2E_SUPPORT
    if (mDecodedPrivateKey.isEmpty()) {
        return false;
    }

    RSA *privateKey = EncryptionUtils::privateKeyFromPEM(mDecodedPrivateKey);
    if (!privateKey) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "decryptRoomsSessionKeys: failed to load private key from PEM";
        return false;
    }

    RoomModel *model = mAccount->roomModel();
    for (Room *r : model->rooms()) {
        if (!r->e2EKey().isEmpty()) {
            r->decryptSessionKeyWithPrivateKey(privateKey);
        }
    }

    RSA_free(privateKey);
#endif
    return true;
}

bool E2eKeyManager::decryptRoomSessionKeys(Room *r) const
{
#if USE_E2E_SUPPORT
    if (mDecodedPrivateKey.isEmpty()) {
        return false;
    }

    RSA *privateKey = EncryptionUtils::privateKeyFromPEM(mDecodedPrivateKey);
    if (!privateKey) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "decryptRoomsSessionKeys: failed to load private key from PEM";
        return false;
    }

    if (!r->e2EKey().isEmpty()) {
        r->decryptSessionKeyWithPrivateKey(privateKey);
    }
    RSA_free(privateKey);
#endif
    return true;
}

#include "moc_e2ekeymanager.cpp"
