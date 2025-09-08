/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
class QSqlTableModel;
class LIBRUQOLACORE_EXPORT E2EDataBase : public LocalDatabaseBase
{
public:
    E2EDataBase();
    ~E2EDataBase() override;

    [[nodiscard]] bool saveKey(const QString &userId, const QByteArray &encryptedPrivateKey, const QByteArray &publicKey);
    [[nodiscard]] bool loadKey(const QString &userId, QByteArray &encryptedPrivateKey, QByteArray &publicKey);
    [[nodiscard]] bool deleteKey(const QString &userId);
    [[nodiscard]] bool hasKey(const QString &userId);

    [[nodiscard]] std::unique_ptr<QSqlTableModel> createAccountsModel(const QString &accountName) const;

protected:
    [[nodiscard]] QString schemaDataBase() const override;
};
