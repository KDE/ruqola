/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolacore_export.h"
#include "localdatabasebase.h"

class QSqlTableModel;
class LIBRUQOLACORE_EXPORT GlobalDatabase : public LocalDatabaseBase
{
    Q_GADGET
public:
    enum class TimeStampType {
        MessageTimeStamp,
        RoomTimeStamp,
        AccountTimeStamp,
        UpdateGlobalRoomsTimeStamp,
        // Etc.
    };
    Q_ENUM(TimeStampType);
    GlobalDatabase();
    ~GlobalDatabase() override;

    void insertOrReplaceTimeStamp(const QString &accountName, const QByteArray &roomId, qint64 timestamp, GlobalDatabase::TimeStampType type);

    [[nodiscard]] QString generateIdentifier(const QString &accountName, const QByteArray &roomId, TimeStampType type);
    void removeTimeStamp(const QString &accountName, const QByteArray &roomId, TimeStampType type);

    [[nodiscard]] qint64 timeStamp(const QString &accountName, const QByteArray &roomId, TimeStampType type);

    [[nodiscard]] std::unique_ptr<QSqlTableModel> createGlobalModel(const QString &accountName) const;

protected:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString schemaDataBase() const override;
};
Q_DECLARE_METATYPE(GlobalDatabase::TimeStampType)
