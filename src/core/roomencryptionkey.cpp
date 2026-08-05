/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomencryptionkey.h"
#include "ruqola_room_memory_debug.h"

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
    mE2EKey = newE2EKey;
    parseSessionKey();
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

    // Extraire encryptedKey
    const QString encryptedKeyBase64 = mE2EKey.mid(36); // ← base64

    // Décoder le base64
    const QByteArray encryptedKey = QByteArray::fromBase64(encryptedKeyBase64.toLatin1());

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

    // TODO: Decrypt with user's RSA private key
    // mSessionKey = EncryptionUtils::decryptSessionKey(encryptedKey, privateKey);
}

QString RoomEncryptionKey::e2eKeyId() const
{
    return mE2eKeyId;
}

void RoomEncryptionKey::setE2eKeyId(const QString &newE2eKeyId)
{
    mE2eKeyId = newE2eKeyId;
}

bool RoomEncryptionKey::operator==(const RoomEncryptionKey &other) const
{
    return other.mE2EKey == mE2EKey && other.mE2eKeyId == mE2eKeyId;
}

QByteArray RoomEncryptionKey::sessionKey() const
{
    return mSessionKey;
}
