/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managelocaldatabase.h"
#include "misc/methodcalljob.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/syncmessagesjob.h"
#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "localdatabase/localdatabasemanager.h"
#include "manageloadhistoryparsesyncmessagesutils.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"
#include "rocketchatbackend.h"
#include "ruqola_loadhistory_debug.h"
#include "ruqolaglobalconfig.h"

#define USE_LOCALDATABASE
ManageLocalDatabase::ManageLocalDatabase(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mRocketChatAccount(account)
{
}

ManageLocalDatabase::~ManageLocalDatabase() = default;

void ManageLocalDatabase::loadAccountSettings()
{
    qCDebug(RUQOLA_LOAD_HISTORY_LOG) << "loadAccountSettings";
    qint64 timeStamp = -1;
#ifdef USE_LOCALDATABASE
    const QString accountName{mRocketChatAccount->accountName()};
    const QByteArray ba = mRocketChatAccount->localDatabaseManager()->jsonAccount(accountName);
    if (!ba.isEmpty()) {
        qCDebug(RUQOLA_LOAD_HISTORY_LOG) << "Account info loads from database";
        mRocketChatAccount->ruqolaServerConfig()->loadAccountSettingsFromLocalDataBase(ba);
        timeStamp = mRocketChatAccount->localDatabaseManager()->timeStamp(accountName, QString(), GlobalDatabase::TimeStampType::AccountTimeStamp);
        qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " timeStamp:" << timeStamp;
    }
#endif
    mRocketChatAccount->rocketChatBackend()->loadPublicSettings(timeStamp);
}

void ManageLocalDatabase::syncMessage(const QByteArray &roomId, qint64 lastSeenAt)
{
    auto job = new RocketChatRestApi::SyncMessagesJob(this);
    job->setRoomId(roomId);
    job->setLastUpdate(QDateTime::fromMSecsSinceEpoch(lastSeenAt));
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::SyncMessagesJob::syncMessagesDone, this, &ManageLocalDatabase::slotSyncMessages);
    if (!job->start()) {
        qCWarning(RUQOLA_LOAD_HISTORY_LOG) << "Couldn't start SyncMessagesJob job";
    }
}

void ManageLocalDatabase::slotSyncMessages(const QJsonObject &obj, const QByteArray &roomId)
{
    qCDebug(RUQOLA_LOAD_HISTORY_LOG) << "roomId" << roomId << "obj" << obj;
    ManageLoadHistoryParseSyncMessagesUtils utils(mRocketChatAccount);
    utils.parse(obj);

    mRocketChatAccount->rocketChatBackend()->addMessagesFromLocalDataBase(utils.updatesMessages());
    mRocketChatAccount->rocketChatBackend()->removeMessageFromLocalDatabase(utils.deletedMessages(), roomId);
}

void ManageLocalDatabase::loadMessagesHistory(const ManageLocalDatabase::ManageLoadHistoryInfo &info)
{
    Q_ASSERT(info.roomModel);

    qint64 endDateTime = info.roomModel->lastTimestamp();
    QJsonArray params;
    params.append(QJsonValue(info.roomId));
    // Load history
    if (info.initial || info.roomModel->isEmpty()) {
        if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
#ifdef USE_LOCALDATABASE
            const QString accountName{mRocketChatAccount->accountName()};
            const QList<Message> lstMessages =
                mRocketChatAccount->localDatabaseManager()->loadMessages(accountName, info.roomName, -1, -1, 50, mRocketChatAccount->emojiManager());
            qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " accountName " << accountName << " roomID " << info.roomId << " info.roomName " << info.roomName
                                             << " number of message " << lstMessages.count();
            if (lstMessages.count() == 50) {
                // Check on network if message change. => we need to add timestamp.
                qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " load from database + update messages";
                mRocketChatAccount->rocketChatBackend()->addMessagesFromLocalDataBase(lstMessages);
                // FIXME: don't use  info.lastSeenAt until we store room information in database
                // We need to use last message timeStamp
                const qint64 endDateTime = info.roomModel->lastTimestamp();
                syncMessage(info.roomId.toLatin1(), /*info.lastSeenAt*/ endDateTime);
                return;
            } else {
                // Load more from network.
                // TODO load missing messages from network
                qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " load from network";
            }
#endif
        }

        params.append(QJsonValue(QJsonValue::Null));
        params.append(QJsonValue(50)); // Max number of messages to load;
        QJsonObject dateObject;
        // qCDebug(RUQOLA_LOAD_HISTORY_LOG) << "roomModel->lastTimestamp()" << roomModel->lastTimestamp() << " ROOMID " << roomID;
        dateObject["$date"_L1] = QJsonValue(info.lastSeenAt);
        params.append(dateObject);
    } else if (info.timeStamp != 0) {
        params.append(info.timeStamp);

        QJsonObject dateObjectEnd;
        dateObjectEnd["$date"_L1] = QJsonValue(endDateTime);

        // qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " QDATE TIME END" << QDateTime::fromMSecsSinceEpoch(endDateTime) << " START "  <<
        // QDateTime::fromMSecsSinceEpoch(startDateTime) << " ROOMID" << roomID;
        params.append(dateObjectEnd);

        params.append(QJsonValue(175)); // Max number of messages to load;
        qDebug() << " params" << params;
    } else {
        const qint64 startDateTime = info.roomModel->generateNewStartTimeStamp(endDateTime);
        int downloadMessage = 50;
        if (RuqolaGlobalConfig::self()->storeMessageInDataBase()) {
#ifdef USE_LOCALDATABASE
            const QString accountName{mRocketChatAccount->accountName()};
            const QList<Message> lstMessages =
                mRocketChatAccount->localDatabaseManager()->loadMessages(accountName, info.roomName, -1, startDateTime, 50, mRocketChatAccount->emojiManager());
            qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " accountName " << accountName << " roomID " << info.roomId << " info.roomName " << info.roomName
                                             << " number of message " << lstMessages.count();
            if (lstMessages.count() == downloadMessage) {
                qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " load from database";
                mRocketChatAccount->rocketChatBackend()->addMessagesFromLocalDataBase(lstMessages);
                return;
            } else if (!lstMessages.isEmpty()) {
                mRocketChatAccount->rocketChatBackend()->addMessagesFromLocalDataBase(lstMessages);
                downloadMessage -= lstMessages.count();
                // Update lastTimeStamp
                endDateTime = info.roomModel->lastTimestamp();
                // TODO load diff messages => 50 - lstMessages.count()
            } else {
                qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " load from network";
            }

#endif
        }
        QJsonObject dateObjectEnd;
        dateObjectEnd["$date"_L1] = QJsonValue(endDateTime);

        // qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " QDATE TIME END" << QDateTime::fromMSecsSinceEpoch(endDateTime) << " START "  <<
        // QDateTime::fromMSecsSinceEpoch(startDateTime) << " ROOMID" << roomID;
        params.append(dateObjectEnd);

        params.append(QJsonValue(downloadMessage)); // Max number of messages to load;

        QJsonObject dateObjectStart;
        // qCDebug(RUQOLA_LOAD_HISTORY_LOG) << "roomModel->lastTimestamp()" << endDateTime << " ROOMID " << roomID;
        dateObjectStart["$date"_L1] = QJsonValue(startDateTime);
        params.append(std::move(dateObjectStart));
    }
    qCDebug(RUQOLA_LOAD_HISTORY_LOG) << " load history ddp:" << params;
#if 0
    mRocketChatAccount->ddp()->loadHistory(params);
#else
    // use /api/v1/method.call/loadHistory directly => restapi
    auto job = new RocketChatRestApi::MethodCallJob(this);
    RocketChatRestApi::MethodCallJob::MethodCallJobInfo loadHistoryInfo;
    loadHistoryInfo.methodName = QStringLiteral("loadHistory");
    loadHistoryInfo.anonymous = false;
    loadHistoryInfo.messageObj = mRocketChatAccount->ddp()->generateJsonObject(loadHistoryInfo.methodName, params);
    job->setMethodCallJobInfo(std::move(loadHistoryInfo));
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    // qDebug()<< " mRestApiConnection " << mRestApiConnection->serverUrl();
    connect(job, &RocketChatRestApi::MethodCallJob::methodCallDone, this, [this](const QJsonObject &root) {
        const QJsonObject obj = root.value("result"_L1).toObject();
        // qCDebug(RUQOLA_DDPAPI_LOG) << obj.value("messages")).toArray().size();
        mRocketChatAccount->rocketChatBackend()->processIncomingMessages(obj.value("messages"_L1).toArray(), true);
    });
    if (!job->start()) {
        qCWarning(RUQOLA_LOAD_HISTORY_LOG) << "Impossible to start loadHistory job";
    }
#endif
    // TODO MISSING load rooms from database too
}

QDebug operator<<(QDebug d, const ManageLocalDatabase::ManageLoadHistoryInfo &t)
{
    d.space() << "roomName" << t.roomName;
    d.space() << "roomID" << t.roomId;
    d.space() << "initial" << t.initial;
    d.space() << "timeStamp" << t.timeStamp;
    d.space() << "lastSeenAt" << t.lastSeenAt;
    d.space() << "roomModel" << t.roomModel;
    return d;
}

#include "moc_managelocaldatabase.cpp"
