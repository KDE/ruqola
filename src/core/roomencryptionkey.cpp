/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomencryptionkey.h"
#include "ruqola_room_memory_debug.h"
#if USE_E2E_SUPPORT
#include "encryption/encryptionutils.h"
#endif

RoomEncryptionKey::RoomEncryptionKey()
{
    qCDebug(RUQOLA_ROOM_MEMORY_LOG) << " RoomEncryptionKey created " << this;
}

RoomEncryptionKey::~RoomEncryptionKey()
{
    qCDebug(RUQOLA_ROOM_MEMORY_LOG) << " RoomEncryptionKey deleted " << this;
}

QString RoomEncryptionKey::e2EKey() const
{
    return mE2EKey;
}

void RoomEncryptionKey::setE2EKey(const QString &newE2EKey)
{
    if (mE2EKey != newE2EKey) {
        mE2EKey = newE2EKey;
        parseSessionKey();
    }
}

void RoomEncryptionKey::parseSessionKey()
{
    if (mE2EKey.isEmpty()) {
        mSessionKey.clear();
        mE2eKeyId.clear();
        return;
    }

    // Format E2EKey: keyId(36) + encryptedKey(base64)
    if (mE2EKey.size() < 36) {
        qCWarning(RUQOLA_ROOM_MEMORY_LOG) << "E2EKey too short:" << mE2EKey.size();
        mSessionKey.clear();
        return;
    }

    mE2eKeyId = mE2EKey.left(36); // ← UUID

    // Extraire encryptedKey (base64)
    mEncryptedKeyBase64 = mE2EKey.mid(36);

    if (mEncryptedKeyBase64.isEmpty()) {
        qCWarning(RUQOLA_ROOM_MEMORY_LOG) << "E2EKey encryptedKey part is empty";
        mSessionKey.clear();
        return;
    }

    // Validate base64 can be decoded
    const QByteArray encryptedKey = QByteArray::fromBase64(mEncryptedKeyBase64.toLatin1());
    if (encryptedKey.isEmpty()) {
        qCWarning(RUQOLA_ROOM_MEMORY_LOG) << "Failed to decode E2EKey from base64";
        mSessionKey.clear();
        return;
    }

    if (encryptedKey.size() != 256) {
        qCWarning(RUQOLA_ROOM_MEMORY_LOG) << "Invalid encryptedKey size:" << encryptedKey.size() << "(expected 256 for RSA-2048)";
        mSessionKey.clear();
        return;
    }

    qDebug() << "E2EKey parsed - keyId:" << mE2eKeyId << "encryptedKey size:" << encryptedKey.size();
    // Waiting for RSA private key to decrypt session key
}

QString RoomEncryptionKey::e2eKeyId() const
{
    return mE2eKeyId;
}

void RoomEncryptionKey::setE2eKeyId(const QString &newE2eKeyId)
{
    mE2eKeyId = newE2eKeyId;
}
#if USE_E2E_SUPPORT
void RoomEncryptionKey::decryptWithPrivateKey(RSA *privateKey)
{
    if (!privateKey) {
        qCWarning(RUQOLA_ROOM_MEMORY_LOG) << "Private key is null, cannot decrypt session key";
        mSessionKey.clear();
        return;
    }

    if (mEncryptedKeyBase64.isEmpty()) {
        qCWarning(RUQOLA_ROOM_MEMORY_LOG) << "No encrypted key available for decryption";
        mSessionKey.clear();
        return;
    }

    // Decode base64 to binary
    const QByteArray encryptedKey = QByteArray::fromBase64(mEncryptedKeyBase64.toLatin1());

    if (encryptedKey.size() != 256) {
        qCWarning(RUQOLA_ROOM_MEMORY_LOG) << "Invalid encryptedKey size for decryption:" << encryptedKey.size();
        mSessionKey.clear();
        return;
    }

    // Decrypt using RSA private key
    mSessionKey = EncryptionUtils::decryptSessionKey(encryptedKey, privateKey);

    if (mSessionKey.isEmpty()) {
        qCWarning(RUQOLA_ROOM_MEMORY_LOG) << "Failed to decrypt session key with private key";
        return;
    }

    if (mSessionKey.size() != 32) {
        qCWarning(RUQOLA_ROOM_MEMORY_LOG) << "Invalid decrypted session key size:" << mSessionKey.size() << "(expected 32)";
        mSessionKey.clear();
        return;
    }

    qDebug() << "Session key successfully decrypted for keyId:" << mE2eKeyId;
}
#endif

bool RoomEncryptionKey::operator==(const RoomEncryptionKey &other) const
{
    return other.mE2EKey == mE2EKey && other.mE2eKeyId == mE2eKeyId;
}

QByteArray RoomEncryptionKey::sessionKey() const
{
    return mSessionKey;
}
