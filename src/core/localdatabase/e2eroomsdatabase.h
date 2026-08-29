/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
class QSqlTableModel;
class LIBRUQOLACORE_EXPORT E2ERoomsDataBase : public LocalDatabaseBase
{
public:
    E2ERoomsDataBase();
    ~E2ERoomsDataBase() override;

    [[nodiscard]] bool
    saveKey(const QString &accountName, const QString &roomId, const QString &keyId, const QByteArray &encryptedPrivateKey, const QByteArray &publicKey);
    [[nodiscard]] bool loadKey(const QString &accountName, const QString &roomId, const QString &keyId, QByteArray &encryptedPrivateKey, QByteArray &publicKey);
    [[nodiscard]] bool deleteKey(const QString &accountName, const QString &roomId, const QString &keyId);
    [[nodiscard]] bool deleteAllKeys(const QString &accountName);
    [[nodiscard]] bool hasKey(const QString &accountName, const QString &roomId, const QString &keyId);

    [[nodiscard]] std::unique_ptr<QSqlTableModel> createE2eRoomsModel(const QString &accountName) const;

    [[nodiscard]] static QString generateRoomKeyId(const QString &roomId, const QString &keyId);

protected:
    [[nodiscard]] QString schemaDataBase() const override;
};
