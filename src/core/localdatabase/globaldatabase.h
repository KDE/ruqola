/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
#pragma once

class LIBRUQOLACORE_EXPORT GlobalDatabase : public LocalDatabaseBase
{
public:
    enum class TimeStampType {
        MessageTimeStamp,
        RoomTimeStamp,
        AccountTimeStamp,
        // Etc.
    };
    GlobalDatabase();
    ~GlobalDatabase() override;

    void updateTimeStamp(const QString &accountName, const QString &roomName, qint64 timestamp, GlobalDatabase::TimeStampType type);

    Q_REQUIRED_RESULT QString generateIdentifier(const QString &accountName, const QString &roomName, TimeStampType type);
    void removeTimeStamp(const QString &accountName, const QString &roomName, TimeStampType type);

    Q_REQUIRED_RESULT qint64 timeStamp(const QString &accountName, const QString &roomName, TimeStampType type);

protected:
    Q_REQUIRED_RESULT QString schemaDataBase() const override;
};
