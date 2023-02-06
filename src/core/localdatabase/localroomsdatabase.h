/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
#pragma once

class LIBRUQOLACORE_EXPORT LocalRoomsDatabase : public LocalDatabaseBase
{
public:
    LocalRoomsDatabase();
    ~LocalRoomsDatabase() override;
    void addRoom(const QString &accountName, const QString &_roomName);
    void deleteRoom(const QString &accountName, const QString &_roomName);

protected:
    Q_REQUIRED_RESULT QString schemaDataBase() const override;
};
