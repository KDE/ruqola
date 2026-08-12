/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "config-ruqola.h"
#include "libruqolacore_export.h"

#include <QObject>
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

    [[nodiscard]] E2eKeyManager::Status needToDecodeEncryptionKey() const;

    [[nodiscard]] QString generateRandomPassword() const;

    [[nodiscard]] Status status() const;
    void setStatus(Status newStatus);

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

Q_SIGNALS:
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
    LIBRUQOLACORE_NO_EXPORT void slotPasswordWritten(QKeychain::Job *baseJob);
    LIBRUQOLACORE_NO_EXPORT void slotPasswordDeleted(QKeychain::Job *baseJob);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString passwordKeyIdentifier() const;
    LIBRUQOLACORE_NO_EXPORT void slotPasswordRead(QKeychain::Job *baseJob);
    LIBRUQOLACORE_NO_EXPORT void distributeRoomSessionKey(const QByteArray &roomId, const QByteArray &sessionKey, const QString &keyId);
    LIBRUQOLACORE_NO_EXPORT void scheduleRequestMissingRoomKeys();
    bool mRequestMissingRoomKeysScheduled = false;
    Status mStatus = Status::Unknown;
    QString mGeneratedPassword;
    QByteArray mDecodedPrivateKey;
    QByteArray mPendingUploadPublicKey;
    QByteArray mPendingUploadPrivateKey;
    bool mPendingUploadFailed = false;
    RocketChatAccount *const mAccount;
};
