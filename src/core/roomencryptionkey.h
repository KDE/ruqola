/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolacore_export.h"
#include <QSharedData>

class LIBRUQOLACORE_EXPORT RoomEncryptionKey : public QSharedData
{
public:
    RoomEncryptionKey();
    ~RoomEncryptionKey();
    [[nodiscard]] QString e2EKey() const;
    void setE2EKey(const QString &newE2EKey);

    [[nodiscard]] QString e2eKeyId() const;
    void setE2eKeyId(const QString &newE2eKeyId);

    [[nodiscard]] bool operator==(const RoomEncryptionKey &other) const;

private:
    // Encryption Key
    QString mE2EKey;
    QString mE2eKeyId;
};
