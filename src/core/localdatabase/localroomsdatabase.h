/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "localdatabasebase.h"
class Room;
class LIBRUQOLACORE_EXPORT LocalRoomsDatabase : public LocalDatabaseBase
{
public:
    LocalRoomsDatabase();
    ~LocalRoomsDatabase() override;
    void updateRoom(const QString &accountName, Room *room);
    void deleteRoom(const QString &accountName, const QString &roomId);

    [[nodiscard]] QByteArray jsonRoom(const QString &accountName, const QString &roomId);

protected:
    [[nodiscard]] QString schemaDataBase() const override;
};
