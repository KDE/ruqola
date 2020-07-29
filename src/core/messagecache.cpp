/*
   Copyright (c) 2020 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "messagecache.h"
#include "restapirequest.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"

#include <chat/getmessagejob.h>
#include <chat/getthreadmessagesjob.h>

#include <model/threadmessagemodel.h>

MessageCache::MessageCache(QObject *parent)
    : QObject(parent)
{
}

MessageCache::~MessageCache()
{

}

ThreadMessageModel *MessageCache::threadMessageModel(const QString &threadMessageId)
{
    ThreadMessageModel *cachedModel = mThreadMessageModels.object(threadMessageId);
    if (cachedModel) {
        return cachedModel;
    } else if (!mThreadMessageJobs.contains(threadMessageId)) {
        // Load the base msg of the thread
        auto *job = new RocketChatRestApi::GetThreadMessagesJob(this);
        mThreadMessageJobs.insert(threadMessageId, job);
        job->setThreadMessageId(threadMessageId);
        connect(job, &RocketChatRestApi::GetThreadMessagesJob::getThreadMessagesDone,
                this, &MessageCache::slotGetThreadMessagesDone);
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
        auto *job = new RocketChatRestApi::GetMessageJob(this);
        mMessageJobs.insert(messageId, job);
        job->setMessageId(messageId);
        connect(job, &RocketChatRestApi::GetMessageJob::getMessageDone,
                this, &MessageCache::slotGetMessageDone);
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
    const QJsonObject msgObject = obj[QStringLiteral("message")].toObject();
    Q_ASSERT(!msgObject.isEmpty());
    Message *message = new Message;
    message->parseMessage(msgObject, true);
    Q_ASSERT(messageId == message->messageId());
    mMessages.insert(message->messageId(), message);
    mMessageJobs.remove(messageId);
    Q_EMIT messageLoaded(message->messageId());
}

bool MessageCache::startJob(RocketChatRestApi::RestApiAbstractJob *job)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    rcAccount->restApi()->initializeRestApiJob(job);
    return job->start();
}
