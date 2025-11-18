/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
class QSqlTableModel;
class LIBRUQOLACORE_EXPORT LocalRoomPendingTypedInfoDatabase : public LocalDatabaseBase
{
public:
    LocalRoomPendingTypedInfoDatabase();
    ~LocalRoomPendingTypedInfoDatabase() override;

    [[nodiscard]] std::unique_ptr<QSqlTableModel> createRoomsModel(const QString &accountName) const;

protected:
    [[nodiscard]] QString schemaDataBase() const override;
};
