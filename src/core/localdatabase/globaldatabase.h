/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
#include <QObject>

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

    void insertOrReplaceTimeStamp(const QString &accountName, const QString &roomName, qint64 timestamp, GlobalDatabase::TimeStampType type);

    Q_REQUIRED_RESULT QString generateIdentifier(const QString &accountName, const QString &roomName, TimeStampType type);
    void removeTimeStamp(const QString &accountName, const QString &roomName, TimeStampType type);

    Q_REQUIRED_RESULT qint64 timeStamp(const QString &accountName, const QString &roomName, TimeStampType type);

protected:
    Q_REQUIRED_RESULT QString schemaDataBase() const override;
};
Q_DECLARE_METATYPE(GlobalDatabase::TimeStampType)
