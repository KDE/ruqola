/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include <QObject>
class RocketChatAccount;
class MessageModel;
class LIBRUQOLACORE_TESTS_EXPORT ManageLocalDatabase : public QObject
{
    Q_OBJECT
public:
    struct ManageLoadHistoryInfo {
        MessageModel *roomModel = nullptr;
        QString roomName;
        QString roomId;
        bool initial = false;
        qint64 timeStamp = -1;
        qint64 lastSeenAt = -1;
    };

    explicit ManageLocalDatabase(RocketChatAccount *account, QObject *parent = nullptr);
    ~ManageLocalDatabase() override;

    void loadHistory(const ManageLocalDatabase::ManageLoadHistoryInfo &info);

private:
    void syncMessage(const QString &roomId, qint64 lastSeenAt);
    void slotSyncMessages(const QJsonObject &obj, const QString &roomId);
    RocketChatAccount *const mRocketChatAccount;
};
Q_DECLARE_TYPEINFO(ManageLocalDatabase::ManageLoadHistoryInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ManageLocalDatabase::ManageLoadHistoryInfo &t);
