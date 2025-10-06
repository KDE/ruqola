/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

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
