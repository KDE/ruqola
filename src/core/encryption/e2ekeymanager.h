/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"

#include <QObject>
class RocketChatAccount;
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

    [[nodiscard]] E2eKeyManager::Status needToDecodeEncryptionKey() const;

    [[nodiscard]] QString generateRandomPassword() const;

    [[nodiscard]] Status status() const;
    void setStatus(Status newStatus);

    [[nodiscard]] bool keySaved() const;
    void setKeySaved(bool newKeySaved);

    void verifyExistingKeyForTest(const QJsonObject &json);

Q_SIGNALS:
    void needDecodeEncryptionKey();
    void failedDecodeEncryptionKey();
    void decodeEncryptionKeyDone();
    void decodeEncryptionKeyPostponed();
    void uploadEncryptionKeyFailed();
    void uploadEncryptionKeyDone();
    void verifyKeyDone();

private:
    LIBRUQOLACORE_NO_EXPORT void verifyExistingKey(const QJsonObject &json);
    LIBRUQOLACORE_NO_EXPORT bool startUploadGeneratedKey(const QByteArray &publicKey, const QByteArray &encryptedPrivateKey);
    Status mStatus = Status::Unknown;
    QString mGeneratedPassword;
    QByteArray mDecodedPrivateKey;
    QByteArray mPendingUploadPublicKey;
    QByteArray mPendingUploadPrivateKey;
    bool mPendingUploadFailed = false;
    RocketChatAccount *const mAccount;
};
