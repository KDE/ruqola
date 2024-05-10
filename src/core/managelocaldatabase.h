/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include <QObject>
class RocketChatAccount;
class MessagesModel;
class LIBRUQOLACORE_TESTS_EXPORT ManageLocalDatabase : public QObject
{
    Q_OBJECT
public:
    struct ManageLoadHistoryInfo {
        MessagesModel *roomModel = nullptr;
        QString roomName;
        QString roomId;
        bool initial = false;
        qint64 timeStamp = -1;
        qint64 lastSeenAt = -1;
    };

    explicit ManageLocalDatabase(RocketChatAccount *account, QObject *parent = nullptr);
    ~ManageLocalDatabase() override;

    void loadMessagesHistory(const ManageLocalDatabase::ManageLoadHistoryInfo &info);

    void loadAccountSettings();

private:
    LIBRUQOLACORE_NO_EXPORT void syncMessage(const QByteArray &roomId, qint64 lastSeenAt);
    LIBRUQOLACORE_NO_EXPORT void slotSyncMessages(const QJsonObject &obj, const QByteArray &roomId);
    RocketChatAccount *const mRocketChatAccount;
};
Q_DECLARE_TYPEINFO(ManageLocalDatabase::ManageLoadHistoryInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ManageLocalDatabase::ManageLoadHistoryInfo &t);
