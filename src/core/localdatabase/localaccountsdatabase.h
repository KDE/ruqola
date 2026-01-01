/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "localdatabasebase.h"
class QSqlTableModel;
class LIBRUQOLACORE_EXPORT LocalAccountsDatabase : public LocalDatabaseBase
{
public:
    LocalAccountsDatabase();
    ~LocalAccountsDatabase() override;

    void deleteAccount(const QString &accountName);
    void updateAccount(const QString &accountName, const QByteArray &ba);

    [[nodiscard]] QByteArray jsonAccount(const QString &accountName);

    [[nodiscard]] std::unique_ptr<QSqlTableModel> createAccountsModel(const QString &accountName) const;

protected:
    [[nodiscard]] QString schemaDataBase() const override;
};
