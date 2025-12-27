/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
class QSqlTableModel;
class LIBRUQOLACORE_EXPORT LocalRoomSubscriptionsDatabase : public LocalDatabaseBase
{
public:
    LocalRoomSubscriptionsDatabase();
    ~LocalRoomSubscriptionsDatabase() override;

    [[nodiscard]] std::unique_ptr<QSqlTableModel> createRoomSubscriptionsModel(const QString &accountName) const;

    void deleteRoomSubscription(const QString &accountName, const QByteArray &subscriptionId);
    void insertRoomSubscription(const QString &accountName, const QByteArray &subscriptionId, const QByteArray &roomId);

    [[nodiscard]] QByteArray roomId(const QString &accountName, const QByteArray &subscriptionId);

protected:
    [[nodiscard]] QString schemaDataBase() const override;
};
