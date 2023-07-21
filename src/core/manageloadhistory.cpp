/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "manageloadhistory.h"
#include "localdatabase/localdatabasemanager.h"
#include "model/messagemodel.h"
#include "rocketchataccount.h"
#include "ruqola_loadhistory_debug.h"
#include "ruqolaglobalconfig.h"

ManageLoadHistory::ManageLoadHistory(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mAccount(account)
{
}

ManageLoadHistory::~ManageLoadHistory() = default;

void ManageLoadHistory::loadHistory(const ManageLoadHistory::ManageLoadHistoryInfo &info)
{
    Q_ASSERT(info.roomModel);

    const qint64 endDateTime = info.roomModel->lastTimestamp();
    // TODO add autotest for it !
    QJsonArray params;
    params.append(QJsonValue(info.roomID));
    // Load history
    if (info.initial || info.roomModel->isEmpty()) {
        if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
            const QString accountName{mAccount->accountName()};
            const QVector<Message> lstMessages = mAccount->localDatabaseManager()->loadMessages(accountName, info.roomID, -1, -1, 50);
            qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " accountName " << accountName << " roomID " << info.roomID << " number of message " << lstMessages.count();
            if (lstMessages.count() < 50) {
                // Load more from network.
            } else {
                // Check on network if message change. => we need to add timestamp.
            }
        }

        params.append(QJsonValue(QJsonValue::Null));
        params.append(QJsonValue(50)); // Max number of messages to load;
        QJsonObject dateObject;
        // qCDebug(RUQOLA_LOAD_HISTORY_LOG) << "roomModel->lastTimestamp()" << roomModel->lastTimestamp() << " ROOMID " << roomID;
        dateObject[QStringLiteral("$date")] = QJsonValue(info.lastSeenAt);
        params.append(dateObject);
    } else if (info.timeStamp != 0) {
        params.append(info.timeStamp);

        QJsonObject dateObjectEnd;
        dateObjectEnd[QStringLiteral("$date")] = QJsonValue(endDateTime);

        // qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " QDATE TIME END" << QDateTime::fromMSecsSinceEpoch(endDateTime) << " START "  <<
        // QDateTime::fromMSecsSinceEpoch(startDateTime) << " ROOMID" << roomID;
        params.append(dateObjectEnd);

        params.append(QJsonValue(175)); // Max number of messages to load;
        qDebug() << " params" << params;
    } else {
        const qint64 startDateTime = info.roomModel->generateNewStartTimeStamp(endDateTime);
        QJsonObject dateObjectEnd;
        dateObjectEnd[QStringLiteral("$date")] = QJsonValue(endDateTime);

        // qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " QDATE TIME END" << QDateTime::fromMSecsSinceEpoch(endDateTime) << " START "  <<
        // QDateTime::fromMSecsSinceEpoch(startDateTime) << " ROOMID" << roomID;
        params.append(dateObjectEnd);

        params.append(QJsonValue(50)); // Max number of messages to load;

        QJsonObject dateObjectStart;
        // qCDebug(RUQOLA_LOAD_HISTORY_LOG) << "roomModel->lastTimestamp()" << endDateTime << " ROOMID " << roomID;
        dateObjectStart[QStringLiteral("$date")] = QJsonValue(startDateTime);
        params.append(std::move(dateObjectStart));
    }
    qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " load history ddp:" << params;
    mAccount->ddp()->loadHistory(params);
}

QDebug operator<<(QDebug d, const ManageLoadHistory::ManageLoadHistoryInfo &t)
{
    d << " roomName " << t.roomName;
    d << " roomID " << t.roomID;
    d << " initial " << t.initial;
    d << " timeStamp " << t.timeStamp;
    d << " lastSeenAt " << t.lastSeenAt;
    d << " roomModel " << t.roomModel;
    return d;
}

#include "moc_manageloadhistory.cpp"
