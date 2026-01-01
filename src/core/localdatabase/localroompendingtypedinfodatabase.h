/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "accountroomsettings.h"
#include "libruqolacore_export.h"
#include "localdatabasebase.h"
#include <QMap>
class QSqlTableModel;
class LIBRUQOLACORE_EXPORT LocalRoomPendingTypedInfoDatabase : public LocalDatabaseBase
{
public:
    LocalRoomPendingTypedInfoDatabase();
    ~LocalRoomPendingTypedInfoDatabase() override;

    [[nodiscard]] std::unique_ptr<QSqlTableModel> createRoomsModel(const QString &accountName) const;

    void updateRoomPendingTypedInfo(const QString &accountName, const QByteArray &roomId, const AccountRoomSettings::PendingTypedInfo &pendingTypedInfo);
    void deleteRoomPendingTypedInfo(const QString &accountName, const QByteArray &roomId);

    [[nodiscard]] QMap<QByteArray /*RoomId*/, AccountRoomSettings::PendingTypedInfo> loadRoomPendingTypedInfo(const QString &accountName);

protected:
    [[nodiscard]] QString schemaDataBase() const override;

private:
    [[nodiscard]] AccountRoomSettings::PendingTypedInfo convertJsonToRoomPendingTypedInfo(const QString &json);
};
