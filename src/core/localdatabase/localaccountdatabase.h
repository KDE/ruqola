/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
#pragma once

class LIBRUQOLACORE_EXPORT LocalAccountDatabase : public LocalDatabaseBase
{
public:
    LocalAccountDatabase();
    ~LocalAccountDatabase() override;

    void deleteAccount(const QString &accountName);
    void updateAccount(const QString &accountName, const QByteArray &ba);

    Q_REQUIRED_RESULT QByteArray jsonAccount(const QString &accountName);

protected:
    Q_REQUIRED_RESULT QString schemaDataBase() const override;
};
