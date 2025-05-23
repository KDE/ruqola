/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
#pragma once

class LIBRUQOLACORE_EXPORT LocalAccountsDatabase : public LocalDatabaseBase
{
public:
    LocalAccountsDatabase();
    ~LocalAccountsDatabase() override;

    void deleteAccount(const QString &accountName);
    void updateAccount(const QString &accountName, const QByteArray &ba);

    [[nodiscard]] QByteArray jsonAccount(const QString &accountName);

protected:
    [[nodiscard]] QString schemaDataBase() const override;
};
