/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ekeymanager.h"
#include "authenticationmanager.h"
#include "config-ruqola.h"
#include "connection.h"
#include "e2e/acceptsuggestedgroupkeyjob.h"
#include "e2e/fetchmykeysjob.h"
#include "e2e/fetchuserswaitingforgroupkeyjob.h"
#include "e2e/getusersofroomwithoutkeyjob.h"
#include "e2e/provideuserswithsuggestedgroupkeysjob.h"
#include "e2e/rejectsuggestedgroupkeyjob.h"
#include "e2e/requestsubscriptionkeysjob.h"
#include "e2e/resetroomkeyjob.h"
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
#include "roomencryptionkey.h"
#include "ruqola_encryption_debug.h"
#include "ruqolaserverconfig.h"
#include <qt6keychain/keychain.h>

#include <QByteArray>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTimeZone>
#include <QTimer>
#include <chrono>
using namespace QKeychain;
using namespace Qt::Literals::StringLiterals;

#if USE_E2E_SUPPORT
namespace
{
// Rocket.Chat hands out public keys serialised as JWK JSON, Ruqola generates PEM ones.
[[nodiscard]] QByteArray resolvePublicKeyPem(const QByteArray &publicKey)
{
    if (publicKey.trimmed().startsWith('{')) {
        const QByteArray publicKeyPem = EncryptionUtils::publicKeyJWKToPEM(publicKey);
        if (publicKeyPem.isEmpty()) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "failed to convert JWK public key to PEM";
        }
        return publicKeyPem;
    }
    return publicKey;
}

// Rocket.Chat expects the RSA-encrypted payload to be the JWK JSON bytes of the session key
// (not raw bytes). Wrap before encrypting.
[[nodiscard]] QByteArray encryptSessionKeyForPublicKey(const QByteArray &sessionKey, const QByteArray &publicKey)
{
    const QByteArray publicKeyPem = resolvePublicKeyPem(publicKey);
    if (publicKeyPem.isEmpty()) {
        return {};
    }
    RSA *rsaPublicKey = EncryptionUtils::publicKeyFromPEM(publicKeyPem);
    if (!rsaPublicKey) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "failed to parse public key";
        return {};
    }
    const QByteArray sessionKeyJwk = EncryptionUtils::sessionKeyToJWK(sessionKey);
    if (sessionKeyJwk.isEmpty()) {
        RSA_free(rsaPublicKey);
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "failed to encode session key as JWK";
        return {};
    }
    const QByteArray encryptedSessionKey = EncryptionUtils::encryptSessionKey(sessionKeyJwk, rsaPublicKey);
    RSA_free(rsaPublicKey);
    return encryptedSessionKey;
}

// Port of Rocket.Chat's encryptOldKeysForParticipant(): each old key is re-encrypted for the
// recipient and keeps its own key id, which is what lets them decrypt the messages of that era.
[[nodiscard]] QVector<RocketChatRestApi::SuggestedOldGroupKey> encryptOldRoomKeysForPublicKey(const QList<RoomEncryptionKey::OldRoomKey> &oldRoomKeys,
                                                                                              const QByteArray &publicKey)
{
    QVector<RocketChatRestApi::SuggestedOldGroupKey> encryptedOldKeys;
    encryptedOldKeys.reserve(oldRoomKeys.size());
    for (const RoomEncryptionKey::OldRoomKey &oldKey : oldRoomKeys) {
        const QByteArray encryptedOldKey = encryptSessionKeyForPublicKey(oldKey.sessionKey, publicKey);
        if (encryptedOldKey.isEmpty()) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "unable to encrypt the old room key" << oldKey.keyId << "for a room member";
            continue;
        }
        // The server stores the date as-is, so an unknown one must not become a 1969 timestamp: it
        // only orders the keys, the lookup goes through the key id.
        const QDateTime timeStamp = oldKey.timeStamp >= 0 ? QDateTime::fromMSecsSinceEpoch(oldKey.timeStamp, QTimeZone::UTC) : QDateTime::currentDateTimeUtc();
        encryptedOldKeys.append({
            oldKey.keyId,
            oldKey.keyId + QString::fromLatin1(encryptedOldKey.toBase64()),
            timeStamp.toString(Qt::ISODateWithMs),
        });
    }
    return encryptedOldKeys;
}

// Rocket.Chat's ROOM_KEY_EXCHANGE_SIZE: number of rooms a single key distribution round covers.
constexpr int roomKeyExchangeSize = 10;
}
#endif

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
    // A leading '{' only *suggests* the V2 envelope: the binary layout starts with a random IV
    // byte, which is '{' once in 256 keys. So never commit to V2 before the JSON really describes
    // one, otherwise those users could never unlock their key again.
    QByteArray decryptedPrivateKey;
    bool decryptedAsV2 = false;
    if (encryptedPrivateKey.startsWith('{')) {
        // ── V2 format (AES-GCM) ─────────────────────────────────────────────
        const QJsonObject v2 = QJsonDocument::fromJson(encryptedPrivateKey).object();
        const QString v2Salt = v2.value(QStringLiteral("salt")).toString();
        const int v2Iterations = v2.value(QStringLiteral("iterations")).toInt();
        const QByteArray v2Iv = QByteArray::fromBase64(v2.value(QStringLiteral("iv")).toString().toUtf8());
        const QByteArray v2Ciphertext = QByteArray::fromBase64(v2.value(QStringLiteral("ciphertext")).toString().toUtf8());

        if (v2Salt.isEmpty() || v2Iterations <= 0 || v2Iv.isEmpty() || v2Ciphertext.isEmpty()) {
            qCDebug(RUQOLA_ENCRYPTION_LOG) << "Encrypted private key is not a V2 envelope, reading it as the binary layout";
        } else {
            const QByteArray v2MasterKey = EncryptionUtils::deriveMasterKey(v2Salt, password, v2Iterations);
            if (v2MasterKey.isEmpty()) {
                setStatus(Status::NeedToDecryptKey);
                Q_EMIT failedDecodeEncryptionKey();
                return false;
            }

            decryptedPrivateKey = EncryptionUtils::decryptAES_GCM_256(v2Ciphertext, v2MasterKey, v2Iv);
            decryptedAsV2 = true;
            if (decryptedPrivateKey.isEmpty()) {
                // Keys a previous Ruqola version sealed derived the master key from the UTF-8
                // encoding of the password, which parts company with every other client as soon as
                // the password is not pure ASCII. Retry that way before giving up — for an ASCII
                // password both derivations agree, so this costs nothing in the common case.
                const QByteArray legacyMasterKey = EncryptionUtils::deriveKey(v2Salt.toUtf8(), password.toUtf8(), v2Iterations, 32);
                if (!legacyMasterKey.isEmpty() && legacyMasterKey != v2MasterKey) {
                    decryptedPrivateKey = EncryptionUtils::decryptAES_GCM_256(v2Ciphertext, legacyMasterKey, v2Iv);
                    if (!decryptedPrivateKey.isEmpty()) {
                        qCWarning(RUQOLA_ENCRYPTION_LOG) << "The private key was sealed with a non-ASCII password encoded the way Ruqola used to: it has "
                                                            "to be re-uploaded before another client can unlock it";
                    }
                }
            }
            if (decryptedPrivateKey.isEmpty()) {
                // The envelope describes itself, so the only thing that can be wrong is the
                // password: this is what a stale keychain entry looks like after the E2E key was
                // reset from another client.
                qCWarning(RUQOLA_ENCRYPTION_LOG) << "The E2E password does not match the stored private key envelope";
            }
        }
    }
    if (!decryptedAsV2) {
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
        privateKeyPem = std::move(decryptedPrivateKey);
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
    mDecodedPrivateKey = std::move(privateKeyPem);
    setStatus(Status::KeyDecrypted);
    const bool sessionKeysDecrypted = decryptRoomsSessionKeys();
    // Suggestions received while the private key was still locked can be imported now, and the
    // rooms left without a key are the ones we have to ask for.
    processSuggestedRoomKeys();
    if (sessionKeysDecrypted) {
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

void E2eKeyManager::deletePassword()
{
    auto deleteJob = new DeletePasswordJob(u"Ruqola"_s);
    connect(deleteJob, &Job::finished, this, &E2eKeyManager::slotPasswordDeleted);
    deleteJob->setKey(passwordKeyIdentifier());
    deleteJob->start();
}

void E2eKeyManager::slotPasswordDeleted(QKeychain::Job *baseJob)
{
    if (baseJob->error() && baseJob->error() != EntryNotFound) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Error deleting password using QKeychain:" << baseJob->errorString();
    }
}

void E2eKeyManager::resetKeys()
{
    mGeneratedPassword.clear();
    mDecodedPrivateKey.clear();
    mPendingUploadPublicKey.clear();
    mPendingUploadPrivateKey.clear();
    mPendingUploadFailed = false;
    mRequestMissingRoomKeysScheduled = false;
    mRoomKeyCreationInProgress.clear();
    stopKeyDistribution();
    setKeySaved(false);
    setStatus(Status::Unknown);
    deletePassword();
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

bool E2eKeyManager::hasUsableKey() const
{
    if (mDecodedPrivateKey.isEmpty()) {
        return false;
    }
    // A freshly generated key is already loaded and usable: only its password still has to be
    // saved by the user, exactly like Rocket.Chat's "SAVE_PASSWORD" ready state.
    return mStatus == Status::KeyDecrypted || mStatus == Status::NeedToGenerateKey;
}

bool E2eKeyManager::initializeRoomE2EKey([[maybe_unused]] const QByteArray &roomId, [[maybe_unused]] const QString &existingKeyId)
{
#if USE_E2E_SUPPORT
    if (!mAccount || roomId.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: invalid arguments";
        return false;
    }
    if (!hasUsableKey()) {
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
        connect(setKeyIdJob, &RocketChatRestApi::SetRoomKeyIDJob::roomKeyIdAlreadyExists, this, [roomId, this]() {
            // Another member won the race and its key is the group key of the room now. Rocket.Chat
            // discards the key it just generated (E2ERoom::createGroupKey() -> discardGroupKey())
            // and waits for the winner to share the real one: distributing ours would replace the
            // key the other members already use.
            qCDebug(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: room key already created by another member, discarding ours" << roomId;
            mRoomKeyCreationInProgress.remove(roomId);
            scheduleRequestMissingRoomKeys();
        });
        connect(setKeyIdJob, &RocketChatRestApi::RestApiAbstractJob::failed, this, [roomId, this]() {
            mRoomKeyCreationInProgress.remove(roomId);
        });
        mRoomKeyCreationInProgress.insert(roomId);
        if (!setKeyIdJob->start()) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "initializeRoomE2EKey: failed to start SetRoomKeyIDJob";
            mRoomKeyCreationInProgress.remove(roomId);
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

    if (!hasUsableKey()) {
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

bool E2eKeyManager::provideRoomKeyToUsers([[maybe_unused]] const QByteArray &roomId, [[maybe_unused]] const QString &keyId)
{
#if USE_E2E_SUPPORT
    // Port of Rocket.Chat's E2ERoom::provideKeyToUser(): a room member which does not own
    // the group key yet asked for it. Re-encrypt our copy for everybody still missing it.
    if (!mAccount || roomId.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "provideRoomKeyToUsers: invalid arguments";
        return false;
    }

    Room *const room = mAccount->room(roomId);
    if (!room) {
        qCDebug(RUQOLA_ENCRYPTION_LOG) << "provideRoomKeyToUsers: unknown room" << roomId;
        return false;
    }

    // The requester tells us which key id it is waiting for. If it doesn't match the one we
    // hold, we are not the right provider (e.g. the room key was reset in the meantime).
    if (room->e2eKeyId() != keyId) {
        qCDebug(RUQOLA_ENCRYPTION_LOG) << "provideRoomKeyToUsers: key id mismatch for room" << roomId << "requested" << keyId << "owned" << room->e2eKeyId();
        return false;
    }

    return distributeExistingRoomE2EKey(roomId);
#else
    return false;
#endif
}

QByteArray E2eKeyManager::ownPublicKey() const
{
    const QString userId = QString::fromLatin1(mAccount->settings()->userId());
    QByteArray encryptedOwnPrivateKey;
    QByteArray ownPublicKeyValue;
    if (!mAccount->localDatabaseManager()->e2EDatabase()->loadKey(mAccount->accountName(), userId, encryptedOwnPrivateKey, ownPublicKeyValue)) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "own public key not found in local database";
        return {};
    }
    return ownPublicKeyValue;
}

void E2eKeyManager::storeRoomSessionKeyLocally([[maybe_unused]] const QByteArray &roomId,
                                               [[maybe_unused]] const QString &keyId,
                                               [[maybe_unused]] const QByteArray &encryptedSessionKey,
                                               [[maybe_unused]] const QByteArray &ownPublicKeyValue)
{
#if USE_E2E_SUPPORT
    // Prime the local room state immediately so the sender can encrypt outgoing
    // messages without waiting for asynchronous server subscription updates.
    if (Room *const room = mAccount->room(roomId)) {
        room->setE2eKeyId(keyId);
        room->setE2EKey(keyId + QString::fromLatin1(encryptedSessionKey.toBase64()));
        if (!decryptRoomSessionKeys(room)) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "storeRoomSessionKeyLocally: unable to decrypt local room session key immediately for" << roomId;
        }
    }

    // Persist the encrypted session key locally so we can decrypt messages without a server round-trip.
    (void)mAccount->localDatabaseManager()->e2ERoomsDataBase()->saveKey(mAccount->accountName(),
                                                                        QString::fromLatin1(roomId),
                                                                        keyId,
                                                                        encryptedSessionKey,
                                                                        ownPublicKeyValue);
#endif
}

void E2eKeyManager::sendRoomKeyToUsers([[maybe_unused]] const QByteArray &roomId,
                                       [[maybe_unused]] const QByteArray &sessionKey,
                                       [[maybe_unused]] const QString &keyId,
                                       [[maybe_unused]] const QJsonArray &users)
{
#if USE_E2E_SUPPORT
    // Port of Rocket.Chat's E2ERoom::encryptGroupKeyForParticipant() applied to a user list.
    if (users.isEmpty()) {
        return;
    }
    const QString ownUserId = QString::fromLatin1(mAccount->settings()->userId());
    // Port of Rocket.Chat's exportOldRoomKeys(): the keys the room used before its current one go
    // out with it, otherwise the member we are onboarding sees nothing written before the last key
    // change. Only the ones we could decrypt can be handed on.
    QList<RoomEncryptionKey::OldRoomKey> oldRoomKeys;
    if (Room *const room = mAccount->room(roomId)) {
        const auto allOldRoomKeys = room->oldRoomKeys();
        for (const RoomEncryptionKey::OldRoomKey &oldKey : allOldRoomKeys) {
            if (!oldKey.sessionKey.isEmpty()) {
                oldRoomKeys.append(oldKey);
            }
        }
    }
    QVector<RocketChatRestApi::SuggestedGroupKey> suggestedKeys;
    suggestedKeys.reserve(users.size());

    for (const QJsonValue &userValue : users) {
        const QJsonObject userObj = userValue.toObject();
        const QString targetUserId = userObj.value("_id"_L1).toString();
        if (targetUserId.isEmpty() || targetUserId == ownUserId) {
            continue;
        }

        QString publicKey = userObj.value("public_key"_L1).toString();
        if (publicKey.isEmpty()) {
            publicKey = userObj.value("e2e"_L1).toObject().value("public_key"_L1).toString();
        }
        if (publicKey.isEmpty()) {
            continue;
        }

        const QByteArray encryptedRecipientSessionKey = encryptSessionKeyForPublicKey(sessionKey, publicKey.toUtf8());
        if (encryptedRecipientSessionKey.isEmpty()) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "sendRoomKeyToUsers: unable to encrypt room key for" << targetUserId;
            continue;
        }

        suggestedKeys.append({
            targetUserId,
            keyId + QString::fromLatin1(encryptedRecipientSessionKey.toBase64()),
            encryptOldRoomKeysForPublicKey(oldRoomKeys, publicKey.toUtf8()),
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
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "sendRoomKeyToUsers: failed to start ProvideUsersWithSuggestedGroupKeysJob for room" << roomId;
    }
#endif
}

void E2eKeyManager::shareRoomKeyWithUsersWithoutKey(const QByteArray &roomId, const QByteArray &sessionKey, const QString &keyId)
{
    // Share this room key with users that do not have a key yet.
    auto usersWithoutKeyJob = new RocketChatRestApi::GetUsersOfRoomWithoutKeyJob(this);
    mAccount->restApi()->initializeRestApiJob(usersWithoutKeyJob);
    usersWithoutKeyJob->setRoomId(roomId);
    connect(usersWithoutKeyJob,
            &RocketChatRestApi::GetUsersOfRoomWithoutKeyJob::getUsersOfRoomWithoutKeyDone,
            this,
            [this, roomId, sessionKey, keyId](const QJsonObject &obj) {
                sendRoomKeyToUsers(roomId, sessionKey, keyId, obj.value("users"_L1).toArray());
            });

    if (!usersWithoutKeyJob->start()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "shareRoomKeyWithUsersWithoutKey: failed to start GetUsersOfRoomWithoutKeyJob for room" << roomId;
    }
}

void E2eKeyManager::distributeRoomSessionKey([[maybe_unused]] const QByteArray &roomId,
                                             [[maybe_unused]] const QByteArray &sessionKey,
                                             [[maybe_unused]] const QString &keyId)
{
#if USE_E2E_SUPPORT
    mRoomKeyCreationInProgress.remove(roomId);
    const QByteArray ownPublicKeyValue = ownPublicKey();
    if (ownPublicKeyValue.isEmpty()) {
        return;
    }

    // Encrypt the session key with own RSA-OAEP public key.
    const QByteArray encryptedSessionKey = encryptSessionKeyForPublicKey(sessionKey, ownPublicKeyValue);
    if (encryptedSessionKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "distributeRoomSessionKey: session key encryption failed";
        return;
    }

    storeRoomSessionKeyLocally(roomId, keyId, encryptedSessionKey, ownPublicKeyValue);

    // Store the encrypted session key in the user's subscription on the server.
    auto updateKeyJob = new RocketChatRestApi::UpdateGroupKeyJob(this);
    mAccount->restApi()->initializeRestApiJob(updateKeyJob);
    RocketChatRestApi::UpdateGroupKeyJob::UpdateGroupKeyInfo updateInfo;
    updateInfo.uid = QString::fromLatin1(mAccount->settings()->userId());
    updateInfo.roomId = QString::fromLatin1(roomId);
    updateInfo.key = QString::fromLatin1(encryptedSessionKey.toBase64());
    updateKeyJob->setUpdateGroupInfo(updateInfo);
    if (!updateKeyJob->start()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "distributeRoomSessionKey: failed to start UpdateGroupKeyJob";
        return;
    }

    shareRoomKeyWithUsersWithoutKey(roomId, sessionKey, keyId);

    qCDebug(RUQOLA_ENCRYPTION_LOG) << "distributeRoomSessionKey: E2E session key initialised for room" << roomId << "keyId" << keyId;
#endif
}

bool E2eKeyManager::resetRoomKey([[maybe_unused]] const QByteArray &roomId)
{
#if USE_E2E_SUPPORT
    if (!mAccount || roomId.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "resetRoomKey: invalid arguments";
        return false;
    }
    if (!hasUsableKey()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "resetRoomKey: E2E key not yet decrypted";
        return false;
    }
    Room *const room = mAccount->room(roomId);
    if (!room) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "resetRoomKey: unknown room" << roomId;
        return false;
    }
    if (room->e2eKeyId().isEmpty()) {
        // The server refuses to reset the key of a room which never had one ("error-room-not-encrypted"):
        // such a room simply needs its first key.
        return initializeRoomE2EKey(roomId);
    }

    const QByteArray ownPublicKeyValue = ownPublicKey();
    if (ownPublicKeyValue.isEmpty()) {
        return false;
    }

    // Port of Rocket.Chat's E2ERoom::resetRoomKey(): generate a brand new session key and key id
    // and hand them to the server encrypted for ourselves only.
    const QByteArray sessionKey = EncryptionUtils::generateSessionKey();
    const QString keyId = EncryptionUtils::generateRoomKeyId();
    const QByteArray encryptedSessionKey = encryptSessionKeyForPublicKey(sessionKey, ownPublicKeyValue);
    if (encryptedSessionKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "resetRoomKey: session key encryption failed for room" << roomId;
        return false;
    }

    auto job = new RocketChatRestApi::ResetRoomKeyJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    const RocketChatRestApi::ResetRoomKeyJob::ResetRoomKeyInfo info{
        .rid = QString::fromLatin1(roomId),
        .e2eKey = keyId + QString::fromLatin1(encryptedSessionKey.toBase64()),
        .e2eKeyId = keyId,
    };
    job->setResetRoomKeyInfo(info);
    connect(job, &RocketChatRestApi::ResetRoomKeyJob::resetRoomKeyDone, this, [this, roomId, keyId, sessionKey, encryptedSessionKey, ownPublicKeyValue]() {
        // The server moved the previous key to "oldRoomKeys", stored ours and queued every
        // other member in "usersWaitingForE2EKeys": they now expect the new key from us.
        storeRoomSessionKeyLocally(roomId, keyId, encryptedSessionKey, ownPublicKeyValue);
        shareRoomKeyWithUsersWithoutKey(roomId, sessionKey, keyId);
        qCDebug(RUQOLA_ENCRYPTION_LOG) << "resetRoomKey: room key reset for" << roomId << "keyId" << keyId;
        Q_EMIT needRefreshView();
        Q_EMIT resetRoomKeyDone(roomId);
    });
    if (!job->start()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "resetRoomKey: failed to start ResetRoomKeyJob for room" << roomId;
        return false;
    }
    return true;
#else
    return false;
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
            QByteArray strBytes = str.toUtf8();

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

            // Oldest format: plain base64 string → binary (iv + ciphertext). Decode strictly: the
            // tolerant mode drops whatever is not base64 and would turn an unknown payload into
            // plausible-looking garbage, which only shows up much later as a failed decryption.
            const auto decoded = QByteArray::fromBase64Encoding(strBytes, QByteArray::Base64Encoding | QByteArray::AbortOnBase64DecodingErrors);
            if (decoded.decodingStatus != QByteArray::Base64DecodingStatus::Ok) {
                // Keep the raw bytes: they are wrong for every format we know, but replacing them
                // with nothing would make us generate and upload a new key pair over the one the
                // server already has.
                qCWarning(RUQOLA_ENCRYPTION_LOG) << "private_key is neither a known JSON envelope nor valid base64";
                return strBytes;
            }
            return decoded.decoded.isEmpty() ? strBytes : decoded.decoded;
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

    const EncryptionUtils::RSAKeyPair rsaKeyPair = EncryptionUtils::generateRSAKey();
    if (rsaKeyPair.privateKey.isEmpty() || rsaKeyPair.publicKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to generate E2E keys: prerequisite generation failed";
        setStatus(Status::Unknown);
        Q_EMIT verifyKeyDone();
        return;
    }

    // OpenSSL gives us PEM, but a Rocket.Chat client stores and shares JWK: the public key it
    // fetches for us must be JWK JSON, otherwise it cannot import it and can never encrypt a room
    // key for us. Same for the private key: what the password protects is its JWK serialisation.
    RSA *const generatedPrivateKey = EncryptionUtils::privateKeyFromPEM(rsaKeyPair.privateKey);
    if (!generatedPrivateKey) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to generate E2E keys: generated private key can't be parsed";
        setStatus(Status::Unknown);
        Q_EMIT verifyKeyDone();
        return;
    }
    const QByteArray publicKeyJwk = EncryptionUtils::exportJWKPublicKey(generatedPrivateKey);
    const QByteArray privateKeyJwk = EncryptionUtils::exportJWKPrivateKey(generatedPrivateKey);
    RSA_free(generatedPrivateKey);
    if (publicKeyJwk.isEmpty() || privateKeyJwk.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to generate E2E keys: JWK export failed";
        setStatus(Status::Unknown);
        Q_EMIT verifyKeyDone();
        return;
    }

    const QByteArray encryptedGeneratedPrivateKey = EncryptionUtils::encryptPrivateKeyV2(privateKeyJwk, mGeneratedPassword, userId);
    if (encryptedGeneratedPrivateKey.isEmpty()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to generate E2E keys: private key encryption failed";
        setStatus(Status::Unknown);
        Q_EMIT verifyKeyDone();
        return;
    }

    // Store exactly what the server will hand back on the next login, so both paths decode the
    // same way.
    (void)mAccount->localDatabaseManager()->e2EDatabase()->saveKey(mAccount->accountName(), userId, encryptedGeneratedPrivateKey, publicKeyJwk);

    // Port of Rocket.Chat's E2E::createAndLoadKeys(): the key we just generated is loaded right
    // away, we don't have to decrypt anything to use it. Without this the account would stay
    // without usable key material until the next login, and no room key could be imported,
    // requested or created (e.g. after an E2E key reset).
    mDecodedPrivateKey = rsaKeyPair.privateKey;
    // We generated the password ourselves, so cache it: after a key reset the keychain entry was
    // removed and nothing would be able to decrypt the key we are about to upload.
    storePassword(mGeneratedPassword);
    // Local key material is ready at this point, so keep generation state even if upload cannot start.
    setStatus(Status::NeedToGenerateKey);
    startUploadGeneratedKey(publicKeyJwk, encryptedGeneratedPrivateKey);
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
    // Rocket.Chat stores the encrypted private key as the JSON envelope itself
    // ({"iv":…,"ciphertext":…,"salt":…,"iterations":…}); the oldest format is a bare base64 blob.
    info.rsaPrivateKey =
        encryptedPrivateKey.trimmed().startsWith('{') ? QString::fromUtf8(encryptedPrivateKey) : QString::fromLatin1(encryptedPrivateKey.toBase64());
    setJob->setSetUserPublicAndPrivateKeysInfo(info);

    connect(setJob, &RocketChatRestApi::SetUserPublicAndPrivateKeysJob::setUserPublicAndPrivateKeysDone, this, [this]() {
        mPendingUploadFailed = false;
        // Our new public key is on the server now, so the other members can encrypt the room keys
        // for us: import what was already shared and ask for what is missing, the way
        // Rocket.Chat's E2E::createAndLoadKeys() ends with requestSubscriptionKeys().
        // Every room key was encrypted with the key this one replaces, so none of them is usable.
        processSuggestedRoomKeys();
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
        // A room can hold nothing but the keys it used before it was re-keyed: they are what its
        // older messages need, so they have to be imported too.
        if (r->hasEncryptedKeys()) {
            r->decryptSessionKeyWithPrivateKey(privateKey);
        }
    }

    RSA_free(privateKey);
#endif
    return true;
}

bool E2eKeyManager::processSuggestedRoomKey([[maybe_unused]] Room *r)
{
#if USE_E2E_SUPPORT
    // Port of Rocket.Chat's E2E.onSubscriptionChanged()/handleAsyncE2EESuggestedKey(): a room
    // member which owns the group key encrypted it for us and stored it in our subscription as
    // "E2ESuggestedKey". Import it, then tell the server whether we accept it — only then does
    // the server promote it to "E2EKey", which is what we need to send encrypted messages.
    if (!mAccount || !r) {
        return false;
    }

    const QString suggestedKey = r->e2ESuggestedKey();
    if (suggestedKey.isEmpty()) {
        // Nothing was shared with us. If this encrypted room has no usable key at all, nobody ever
        // sent us one: run the Rocket.Chat E2ERoom::handshake() recovery for it.
        if (r->encrypted() && r->sessionKey().isEmpty() && hasUsableKey()) {
            scheduleRequestMissingRoomKeys();
        }
        return false;
    }

    if (!hasUsableKey()) {
        // Our own private key is not available yet: keep the suggestion around, it will be
        // retried from processSuggestedRoomKeys() once the key is decrypted.
        qCDebug(RUQOLA_ENCRYPTION_LOG) << "processSuggestedRoomKey: private key not ready, postponing for room" << r->roomId();
        return false;
    }

    RSA *privateKey = EncryptionUtils::privateKeyFromPEM(mDecodedPrivateKey);
    if (!privateKey) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "processSuggestedRoomKey: failed to load private key from PEM";
        return false;
    }

    // Try the import on a scratch object so a bogus suggestion cannot destroy a room key we
    // already own and use.
    RoomEncryptionKey candidate;
    candidate.setE2EKey(suggestedKey);
    candidate.decryptWithPrivateKey(privateKey);
    RSA_free(privateKey);

    const bool imported = !candidate.sessionKey().isEmpty();
    const QByteArray roomId = r->roomId();

    if (imported) {
        // The suggestion is "keyId + base64(ciphertext)": adopt the key id it carries, but never
        // clear a known one when the payload had no prefix.
        if (!candidate.e2eKeyId().isEmpty()) {
            r->setE2eKeyId(candidate.e2eKeyId());
        }
        r->setE2EKey(suggestedKey);
        if (!decryptRoomSessionKeys(r)) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "processSuggestedRoomKey: unable to decrypt imported key for room" << roomId;
        }
        auto acceptJob = new RocketChatRestApi::AcceptSuggestedGroupKeyJob(this);
        mAccount->restApi()->initializeRestApiJob(acceptJob);
        acceptJob->setRoomId(QString::fromLatin1(roomId));
        if (!acceptJob->start()) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "processSuggestedRoomKey: failed to start AcceptSuggestedGroupKeyJob for room" << roomId;
        }
        qCDebug(RUQOLA_ENCRYPTION_LOG) << "processSuggestedRoomKey: accepted suggested key for room" << roomId << "keyId" << candidate.e2eKeyId();
    } else {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "processSuggestedRoomKey: invalid suggested key for room" << roomId << ", rejecting it";
        auto rejectJob = new RocketChatRestApi::RejectSuggestedGroupKeyJob(this);
        mAccount->restApi()->initializeRestApiJob(rejectJob);
        rejectJob->setRoomId(QString::fromLatin1(roomId));
        if (!rejectJob->start()) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "processSuggestedRoomKey: failed to start RejectSuggestedGroupKeyJob for room" << roomId;
        }
    }

    // Handled either way: the server clears the suggestion, so drop our local copy too.
    r->setE2ESuggestedKey({});
    return imported;
#else
    return false;
#endif
}

void E2eKeyManager::processSuggestedRoomKeys()
{
#if USE_E2E_SUPPORT
    if (!mAccount) {
        return;
    }
    const auto rooms = mAccount->roomModel()->rooms();
    for (Room *r : rooms) {
        // Rooms left without a key request or create it themselves.
        (void)processSuggestedRoomKey(r);
    }
    // Rocket.Chat's E2E::onE2EEReady() ends with initiateKeyDistribution(): our key material is
    // usable, so we can serve the members which are waiting for the room keys we own.
    startKeyDistribution();
#endif
}

void E2eKeyManager::startKeyDistribution()
{
    if (mKeyDistributionTimer) {
        return;
    }
    // Rocket.Chat runs its key distribution every 10s (E2E::initiateKeyDistribution()).
    mKeyDistributionTimer = new QTimer(this);
    mKeyDistributionTimer->setInterval(std::chrono::seconds{10});
    connect(mKeyDistributionTimer, &QTimer::timeout, this, &E2eKeyManager::distributeKeysToWaitingUsers);
    mKeyDistributionTimer->start();
    distributeKeysToWaitingUsers();
}

void E2eKeyManager::stopKeyDistribution()
{
    delete mKeyDistributionTimer;
    mKeyDistributionTimer = nullptr;
}

void E2eKeyManager::distributeKeysToWaitingUsers()
{
#if USE_E2E_SUPPORT
    if (!mAccount || !hasUsableKey() || mAccount->loginStatus() != AuthenticationManager::LoginStatus::LoggedIn) {
        return;
    }
    // Port of Rocket.Chat's keyDistribution(): the server queues in "usersWaitingForE2EKeys" the
    // members of a room which have no key for it (new members, members which reset their own E2E
    // key…). Only the rooms we own a session key for can be served, and a room where we are
    // waiting ourselves is somebody else's job.
    const QByteArray ownUserId = mAccount->settings()->userId();
    QList<QByteArray> roomIds;
    const auto rooms = mAccount->roomModel()->rooms();
    for (Room *r : rooms) {
        const QList<QByteArray> waitingUsers = r->usersWaitingForE2EKeys();
        if (waitingUsers.isEmpty() || waitingUsers.contains(ownUserId)) {
            continue;
        }
        if (r->sessionKey().isEmpty() || r->e2eKeyId().isEmpty()) {
            continue;
        }
        roomIds.append(r->roomId());
        if (roomIds.count() == roomKeyExchangeSize) {
            break;
        }
    }
    if (roomIds.isEmpty()) {
        return;
    }

    auto job = new RocketChatRestApi::FetchUsersWaitingForGroupKeyJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    job->setRoomIds(roomIds);
    connect(job, &RocketChatRestApi::FetchUsersWaitingForGroupKeyJob::fetchUsersWaitingForGroupKeyDone, this, &E2eKeyManager::slotUsersWaitingForGroupKey);
    if (!job->start()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to start FetchUsersWaitingForGroupKeyJob";
    }
#endif
}

void E2eKeyManager::slotUsersWaitingForGroupKey([[maybe_unused]] const QJsonObject &replyObject)
{
#if USE_E2E_SUPPORT
    if (!mAccount) {
        return;
    }
    const QJsonObject usersPerRoom = replyObject.value("usersWaitingForE2EKeys"_L1).toObject();
    for (auto it = usersPerRoom.constBegin(); it != usersPerRoom.constEnd(); ++it) {
        const QByteArray roomId = it.key().toLatin1();
        Room *const r = mAccount->room(roomId);
        if (!r) {
            continue;
        }
        const QByteArray sessionKey = r->sessionKey();
        if (sessionKey.isEmpty()) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "slotUsersWaitingForGroupKey: no session key to share for room" << roomId;
            continue;
        }
        sendRoomKeyToUsers(roomId, sessionKey, r->e2eKeyId(), it.value().toArray());
    }
#endif
}

void E2eKeyManager::scheduleRequestMissingRoomKeys()
{
    if (mRequestMissingRoomKeysScheduled) {
        return;
    }
    mRequestMissingRoomKeysScheduled = true;
    // Rooms arrive in batches: coalesce them into a single request.
    QTimer::singleShot(std::chrono::seconds{2}, this, [this]() {
        mRequestMissingRoomKeysScheduled = false;
        requestMissingRoomKeys();
    });
}

void E2eKeyManager::requestMissingRoomKeys()
{
    if (!mAccount) {
        return;
    }
    // Rocket.Chat's E2ERoom::handshake() publishes "<roomId>/e2ekeyRequest" per room; the REST
    // endpoint does the same server-side for every subscription of ours which has no key yet.
    // Members owning the key answer with e2e.provideUsersSuggestedGroupKeys, which comes back
    // to us as an "E2ESuggestedKey" handled by processSuggestedRoomKey().
    auto job = new RocketChatRestApi::RequestSubscriptionKeysJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(RUQOLA_ENCRYPTION_LOG) << "Unable to start RequestSubscriptionKeysJob";
    }

    // The endpoint above only covers the rooms which already have a key id. A room without one has
    // no group key at all (e.g. encryption was enabled on an existing room): the first member
    // noticing it creates the key, as E2ERoom::handshake() does when room.e2eKeyId is unset.
    // The key id comes from the rooms payload, not from the subscriptions: this runs delayed so it
    // cannot mistake a room whose payload did not arrive yet for a room without key.
    if (!hasUsableKey()) {
        return;
    }
    const auto rooms = mAccount->roomModel()->rooms();
    for (Room *r : rooms) {
        if (!r->encrypted() || !r->sessionKey().isEmpty() || !r->e2eKeyId().isEmpty()) {
            continue;
        }
        if (mRoomKeyCreationInProgress.contains(r->roomId())) {
            continue;
        }
        if (!initializeRoomE2EKey(r->roomId())) {
            qCWarning(RUQOLA_ENCRYPTION_LOG) << "requestMissingRoomKeys: unable to create the group key of room" << r->roomId();
        }
    }
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

    if (r->hasEncryptedKeys()) {
        r->decryptSessionKeyWithPrivateKey(privateKey);
    }
    RSA_free(privateKey);
#endif
    return true;
}

#include "moc_e2ekeymanager.cpp"
