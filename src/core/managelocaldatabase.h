/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include <QHash>
#include <QObject>
#include <QSet>
class QJsonArray;
class RocketChatAccount;
class MessagesModel;
class LIBRUQOLACORE_TESTS_EXPORT ManageLocalDatabase : public QObject
{
    Q_OBJECT
public:
    struct ManageLoadHistoryInfo {
        MessagesModel *roomModel = nullptr;
        QString roomName;
        QByteArray roomId;
        bool initial = false;
        qint64 timeStamp = 0;
        qint64 lastSeenAt = -1;
    };

    explicit ManageLocalDatabase(RocketChatAccount *account, QObject *parent = nullptr);
    ~ManageLocalDatabase() override;

    void loadMessagesHistory(const ManageLocalDatabase::ManageLoadHistoryInfo &info);

    void loadAccountSettings();

    // rooms.history "previous" cursor: timestamp (ms) of the oldest message loaded from the server
    static constexpr qint64 HistoryCursorUnknown = -1; // no cursor known, fall back to loadHistory method
    static constexpr qint64 HistoryCursorEnd = 0; // oldest message of the room already loaded

    [[nodiscard]] static qint64 parseHistoryCursor(const QJsonObject &root);

    [[nodiscard]] qint64 historyCursor(const QByteArray &roomId);
    void setHistoryCursor(const QByteArray &roomId, qint64 cursor);

private:
    LIBRUQOLACORE_NO_EXPORT void loadRoomHistory(const QByteArray &roomId, int count, qint64 lastSeenAt, qint64 previousCursor);
    LIBRUQOLACORE_NO_EXPORT void loadHistoryMethod(const QByteArray &roomId, const QJsonArray &params);
    LIBRUQOLACORE_NO_EXPORT void trackHistoryJob(QObject *job, const QByteArray &roomId);
    LIBRUQOLACORE_NO_EXPORT void syncMessage(const QByteArray &roomId, qint64 lastSeenAt);
    LIBRUQOLACORE_NO_EXPORT void slotSyncMessages(const QJsonObject &obj, const QByteArray &roomId);
    QHash<QByteArray, qint64> mHistoryCursors;
    // Rooms with a history request in progress (avoid flooding the server while scrolling)
    QSet<QByteArray> mLoadingHistoryRooms;
    RocketChatAccount *const mRocketChatAccount;
};
Q_DECLARE_TYPEINFO(ManageLocalDatabase::ManageLoadHistoryInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ManageLocalDatabase::ManageLoadHistoryInfo &t);
