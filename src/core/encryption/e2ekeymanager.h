/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "config-ruqola.h"
#include "libruqolacore_export.h"

#include <QObject>
#include <QSet>
class QTimer;
class QJsonArray;
class QJsonObject;
class RocketChatAccount;
namespace QKeychain
{
class Job;
}
class Room;
class LIBRUQOLACORE_EXPORT E2eKeyManager : public QObject
{
    Q_OBJECT
public:
    enum class Status : uint8_t {
        Unknown = 0,
        NeedToDecryptKey,
        NeedToGenerateKey,
        KeyDecrypted,
        DecryptionPostponned,
    };
    Q_ENUM(Status)
    explicit E2eKeyManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~E2eKeyManager() override;

    void decodeEncryptionKey();
    [[nodiscard]] bool decodeEncryptionKey(const QString &password);
    void postponeDecryption();
    [[nodiscard]] bool retryUploadGeneratedKey();
    [[nodiscard]] bool hasPendingUploadFailure() const;

    void fetchMyKeys();
    // Forget every trace of the current key: cached key material, the E2E password kept in the
    // keychain (it cannot decrypt the key the server hands out after a reset) and the status.
    void resetKeys();
    [[nodiscard]] bool initializeRoomE2EKey(const QByteArray &roomId, const QString &existingKeyId = {});
    [[nodiscard]] bool distributeExistingRoomE2EKey(const QByteArray &roomId);
    [[nodiscard]] bool provideRoomKeyToUsers(const QByteArray &roomId, const QString &keyId);

    // Replace the group key of a room by a brand new one and hand it out to the other members.
    // Last resort when nobody is able to share the current key anymore (e.g. every member reset
    // its own E2E key): the messages encrypted with the previous key stay unreadable.
    [[nodiscard]] bool resetRoomKey(const QByteArray &roomId);

    [[nodiscard]] E2eKeyManager::Status needToDecodeEncryptionKey() const;

    [[nodiscard]] QString generateRandomPassword() const;

    [[nodiscard]] Status status() const;
    void setStatus(Status newStatus);

    // Port of Rocket.Chat's E2E::isReady(): our own key material can be used to encrypt/decrypt
    // room keys. A key we generated ourselves is usable as soon as it exists, even though the
    // user still has to save its password ("NeedToGenerateKey" == Rocket.Chat "SAVE_PASSWORD").
    [[nodiscard]] bool hasUsableKey() const;

    [[nodiscard]] bool keySaved() const;
    void setKeySaved(bool newKeySaved);

    void verifyExistingKeyForTest(const QJsonObject &json);
    [[nodiscard]] bool decryptRoomsSessionKeys() const;
    [[nodiscard]] bool decryptRoomSessionKeys(Room *r) const;

    // Import the room key another member encrypted for us ("E2ESuggestedKey") and tell the
    // server whether we accept it. Returns true when a key was successfully imported.
    [[nodiscard]] bool processSuggestedRoomKey(Room *r);
    void processSuggestedRoomKeys();

    // Ask the members of the encrypted rooms we have no key for to share it with us.
    void requestMissingRoomKeys();

    // Share the room keys we own with the members the server queued as waiting for them.
    void distributeKeysToWaitingUsers();

Q_SIGNALS:
    void resetRoomKeyDone(const QByteArray &roomId);
    void needDecodeEncryptionKey();
    void failedDecodeEncryptionKey();
    void decodeEncryptionKeyDone();
    void decodeEncryptionKeyPostponed();
    void uploadEncryptionKeyFailed();
    void uploadEncryptionKeyDone();
    void verifyKeyDone();
    void needRefreshView();

private:
    LIBRUQOLACORE_NO_EXPORT void readPassword();
    LIBRUQOLACORE_NO_EXPORT void verifyExistingKey(const QJsonObject &json);
    LIBRUQOLACORE_NO_EXPORT bool startUploadGeneratedKey(const QByteArray &publicKey, const QByteArray &encryptedPrivateKey);
    LIBRUQOLACORE_NO_EXPORT void storePassword(const QString &password);
    LIBRUQOLACORE_NO_EXPORT void deletePassword();
    static LIBRUQOLACORE_NO_EXPORT void slotPasswordWritten(QKeychain::Job *baseJob);
    static LIBRUQOLACORE_NO_EXPORT void slotPasswordDeleted(QKeychain::Job *baseJob);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString passwordKeyIdentifier() const;
    LIBRUQOLACORE_NO_EXPORT void slotPasswordRead(QKeychain::Job *baseJob);
    LIBRUQOLACORE_NO_EXPORT void distributeRoomSessionKey(const QByteArray &roomId, const QByteArray &sessionKey, const QString &keyId);
    LIBRUQOLACORE_NO_EXPORT void scheduleRequestMissingRoomKeys();
    // Own public key as stored locally: PEM when we generated it, JWK JSON when it comes from
    // another Rocket.Chat client.
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QByteArray ownPublicKey() const;
    LIBRUQOLACORE_NO_EXPORT void
    storeRoomSessionKeyLocally(const QByteArray &roomId, const QString &keyId, const QByteArray &encryptedSessionKey, const QByteArray &ownPublicKeyValue);
    // Encrypt the room key with the public key of every listed member and store it as a suggested
    // key in their subscription. Accepts both user list layouts used by the server: "public_key"
    // (e2e.fetchUsersWaitingForGroupKey) and "e2e.public_key" (e2e.getUsersOfRoomWithoutKey).
    LIBRUQOLACORE_NO_EXPORT void sendRoomKeyToUsers(const QByteArray &roomId, const QByteArray &sessionKey, const QString &keyId, const QJsonArray &users);
    LIBRUQOLACORE_NO_EXPORT void shareRoomKeyWithUsersWithoutKey(const QByteArray &roomId, const QByteArray &sessionKey, const QString &keyId);
    LIBRUQOLACORE_NO_EXPORT void startKeyDistribution();
    LIBRUQOLACORE_NO_EXPORT void stopKeyDistribution();
    LIBRUQOLACORE_NO_EXPORT void slotUsersWaitingForGroupKey(const QJsonObject &replyObject);
    // Rooms for which we already asked the server to register a key id: creating a second one
    // would fight with the first attempt (Rocket.Chat uses the CREATING_KEYS room state).
    QSet<QByteArray> mRoomKeyCreationInProgress;
    QTimer *mKeyDistributionTimer = nullptr;
    bool mRequestMissingRoomKeysScheduled = false;
    Status mStatus = Status::Unknown;
    QString mGeneratedPassword;
    QByteArray mDecodedPrivateKey;
    QByteArray mPendingUploadPublicKey;
    QByteArray mPendingUploadPrivateKey;
    bool mPendingUploadFailed = false;
    RocketChatAccount *const mAccount;
};
