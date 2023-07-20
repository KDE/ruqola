/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "manageloadhistory.h"
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

void ManageLoadHistory::loadHistory(MessageModel *roomModel, const QString &roomID, bool initial, qint64 timeStep)
{
    Q_ASSERT(roomModel);

    const qint64 endDateTime = roomModel->lastTimestamp();
    // TODO add autotest for it !
    QJsonArray params;
    params.append(QJsonValue(roomID));
    // Load history
    if (initial || roomModel->isEmpty()) {
        if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
            // TODO
        }

        params.append(QJsonValue(QJsonValue::Null));
        params.append(QJsonValue(50)); // Max number of messages to load;
        QJsonObject dateObject;
        // qCDebug(RUQOLA_LOAD_HISTORY_LOG) << "roomModel->lastTimestamp()" << roomModel->lastTimestamp() << " ROOMID " << roomID;
        dateObject[QStringLiteral("$date")] = QJsonValue(endDateTime);
        params.append(dateObject);
    } else if (timeStep != 0) {
        QJsonObject dateObjectEnd;
        dateObjectEnd[QStringLiteral("$date")] = QJsonValue(endDateTime);

        // qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " QDATE TIME END" << QDateTime::fromMSecsSinceEpoch(endDateTime) << " START "  <<
        // QDateTime::fromMSecsSinceEpoch(startDateTime) << " ROOMID" << roomID;
        params.append(dateObjectEnd);

        params.append(QJsonValue(175)); // Max number of messages to load;

        QJsonObject dateObjectStart;
        qDebug() << "roomModel->lastTimestamp()" << endDateTime << " ROOMID " << roomID << " timeStep " << timeStep;
        // dateObjectStart[QStringLiteral("$date")] = QJsonValue(timeStep);
        // params.append(dateObjectStart);
        qDebug() << " params" << params;
    } else {
        const qint64 startDateTime = roomModel->generateNewStartTimeStamp(endDateTime);
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
