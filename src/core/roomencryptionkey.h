/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "config-ruqola.h"
#include "libruqolacore_export.h"
#include <QList>
#include <QSharedData>
class QJsonArray;
#if USE_E2E_SUPPORT
extern "C" {
#include <openssl/rsa.h>
}
#endif
class LIBRUQOLACORE_EXPORT RoomEncryptionKey : public QSharedData
{
public:
    // A key the room used before it was re-keyed. Messages carry the id of the key they were
    // written with, so keeping the previous ones is the only way to still read that history.
    struct OldRoomKey {
        QString keyId;
        QString encryptedKeyBase64;
        QByteArray sessionKey;
        // When the key stopped being the room key. Rocket.Chat requires it back when the key is
        // handed on to another member, so it has to survive the round trip.
        qint64 timeStamp = -1;
    };

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

    // Keys the room used before its current one, from the subscription fields "oldRoomKeys" (ours
    // already) and "suggestedOldRoomKeys" (still offered to us). Each is RSA-encrypted for us.
    void parseOldRoomKeys(const QJsonArray &array);

    [[nodiscard]] bool operator==(const RoomEncryptionKey &other) const;

    [[nodiscard]] QByteArray sessionKey() const;

    // The key a message encrypted under 'keyId' has to be decrypted with, current or older.
    [[nodiscard]] QByteArray sessionKeyForKeyId(const QString &keyId) const;

    [[nodiscard]] QList<OldRoomKey> oldRoomKeys() const;

    // Whether any key of the room, current or older, is usable.
    [[nodiscard]] bool hasSessionKey() const;

    // Whether there is key material waiting for our private key.
    [[nodiscard]] bool hasEncryptedKeys() const;

#if USE_E2E_SUPPORT
    // Decrypt the session key using the provided RSA private key
    // This is called after E2EKey is received from DDP
    void decryptWithPrivateKey(RSA *privateKey);
#endif
private:
    LIBRUQOLACORE_NO_EXPORT void parseSessionKey();
    LIBRUQOLACORE_NO_EXPORT void retireCurrentKey();
#if USE_E2E_SUPPORT
    LIBRUQOLACORE_NO_EXPORT void decryptOldRoomKeysWithPrivateKey(RSA *privateKey);
#endif
    // Encryption Key
    QString mE2EKey;
    QString mE2ESuggestedKey;
    QString mE2eKeyId;
    QString mEncryptedKeyBase64; // Base64-encoded RSA-encrypted session key
    QByteArray mSessionKey;
    QList<OldRoomKey> mOldRoomKeys;
    QList<QByteArray> mUsersWaitingForE2EKeys;
};
