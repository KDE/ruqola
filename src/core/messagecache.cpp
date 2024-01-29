/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagecache.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"

#include "chat/getmessagejob.h"
#include "chat/getthreadmessagesjob.h"

#include "model/threadmessagemodel.h"

MessageCache::MessageCache(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
}

MessageCache::~MessageCache() = default;

ThreadMessageModel *MessageCache::threadMessageModel(const QString &threadMessageId)
{
    ThreadMessageModel *cachedModel = mThreadMessageModels.object(threadMessageId);
    if (cachedModel) {
        return cachedModel;
    } else if (!mThreadMessageJobs.contains(threadMessageId)) {
        // Load the base msg of the thread
        auto job = new RocketChatRestApi::GetThreadMessagesJob(this);
        mThreadMessageJobs.insert(threadMessageId, job);
        job->setThreadMessageId(threadMessageId);
        connect(job, &RocketChatRestApi::GetThreadMessagesJob::getThreadMessagesDone, this, &MessageCache::slotGetThreadMessagesDone);
        if (!startJob(job)) {
            qCDebug(RUQOLA_LOG) << "Impossible to start GetThreadMessagesJob";
        }
    }
    return nullptr;
}

Message *MessageCache::messageForId(const QString &messageId)
{
    Message *cachedMessage = mMessages.object(messageId);
    if (cachedMessage) {
        return cachedMessage;
    } else if (!mMessageJobs.contains(messageId)) {
        auto job = new RocketChatRestApi::GetMessageJob(this);
        mMessageJobs.insert(messageId, job);
        job->setMessageId(messageId);
        connect(job, &RocketChatRestApi::GetMessageJob::getMessageDone, this, &MessageCache::slotGetMessageDone);
        if (!startJob(job)) {
            qCDebug(RUQOLA_LOG) << "Impossible to start GetMessageJob";
        }
    }
    return nullptr;
}

void MessageCache::slotGetThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId)
{
    ThreadMessageModel *model = mThreadMessageModels.object(threadMessageId);
    if (!model) {
        model = new ThreadMessageModel;
        model->parseThreadMessages(obj);
        mThreadMessageModels.insert(threadMessageId, model);
    } else {
        model->loadMoreThreadMessages(obj);
    }
    mThreadMessageJobs.remove(threadMessageId);
    Q_EMIT modelLoaded();
}

void MessageCache::slotGetMessageDone(const QJsonObject &obj, const QString &messageId)
{
    const QJsonObject msgObject = obj[QLatin1String("message")].toObject();
    Q_ASSERT(!msgObject.isEmpty());
    auto message = new Message;
    message->parseMessage(msgObject, true);
    Q_ASSERT(messageId == message->messageId());
    mMessages.insert(message->messageId(), message);
    mMessageJobs.remove(messageId);
    Q_EMIT messageLoaded(message->messageId());
}

bool MessageCache::startJob(RocketChatRestApi::RestApiAbstractJob *job)
{
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    return job->start();
}

#include "moc_messagecache.cpp"
