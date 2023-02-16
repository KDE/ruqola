/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
#pragma once

class LIBRUQOLACORE_EXPORT LocalGlobalDatabase : public LocalDatabaseBase
{
public:
    enum class TimeStampType {
        MessageTimeStamp,
        RoomTimeStamp,
        // Etc.
    };
    LocalGlobalDatabase();
    ~LocalGlobalDatabase() override;

    void updateTimeStamp(const QString &accountName, const QString &roomName, qint64 timestamp, LocalGlobalDatabase::TimeStampType type);

protected:
    Q_REQUIRED_RESULT QString schemaDataBase() const override;
};
