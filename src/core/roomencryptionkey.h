/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "config-ruqola.h"
#include "libruqolacore_export.h"
#include <QList>
#include <QSharedData>
#if USE_E2E_SUPPORT
extern "C" {
#include <openssl/rsa.h>
}
#endif
class LIBRUQOLACORE_EXPORT RoomEncryptionKey : public QSharedData
{
public:
    RoomEncryptionKey();
    ~RoomEncryptionKey();
    [[nodiscard]] QString e2EKey() const;
    void setE2EKey(const QString &newE2EKey);

    [[nodiscard]] QString e2eKeyId() const;
    void setE2eKeyId(const QString &newE2eKeyId);

    // Key another room member encrypted for us and which is still pending our approval
    // (subscription field "E2ESuggestedKey"). It becomes the E2EKey once accepted.
    [[nodiscard]] QString e2ESuggestedKey() const;
    void setE2ESuggestedKey(const QString &newE2ESuggestedKey);

    // Members which have no key for the room yet and expect the ones owning it to share it with
    // them (room field "usersWaitingForE2EKeys").
    [[nodiscard]] QList<QByteArray> usersWaitingForE2EKeys() const;
    void setUsersWaitingForE2EKeys(const QList<QByteArray> &newUsersWaitingForE2EKeys);

    [[nodiscard]] bool operator==(const RoomEncryptionKey &other) const;

    [[nodiscard]] QByteArray sessionKey() const;

#if USE_E2E_SUPPORT
    // Decrypt the session key using the provided RSA private key
    // This is called after E2EKey is received from DDP
    void decryptWithPrivateKey(RSA *privateKey);
#endif
private:
    LIBRUQOLACORE_NO_EXPORT void parseSessionKey();
    // Encryption Key
    QString mE2EKey;
    QString mE2ESuggestedKey;
    QString mE2eKeyId;
    QString mEncryptedKeyBase64; // Base64-encoded RSA-encrypted session key
    QByteArray mSessionKey;
    QList<QByteArray> mUsersWaitingForE2EKeys;
};
