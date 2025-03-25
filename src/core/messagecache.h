/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "misc/methodcalljob.h"
#include "model/threadmessagemodel.h"
#include <QCache>
#include <QMap>
#include <QObject>

class Message;
namespace RocketChatRestApi
{
class RestApiAbstractJob;
class GetThreadMessagesJob;
class GetMessageJob;
}

class LIBRUQOLACORE_EXPORT MessageCache : public QObject
{
    Q_OBJECT
public:
    explicit MessageCache(RocketChatAccount *account, QObject *parent = nullptr);
    ~MessageCache() override;

    [[nodiscard]] ThreadMessageModel *threadMessageModel(const QByteArray &threadMessageId);
    [[nodiscard]] Message *messageForId(const QByteArray &messageId);

Q_SIGNALS:
    void modelLoaded();
    void messageLoaded(const QByteArray &threadMessageId);

protected:
    virtual bool startJob(RocketChatRestApi::RestApiAbstractJob *job); // virtual for unittest
    virtual RocketChatRestApi::MethodCallJob::MethodCallJobInfo generateMethodCallInfo(const QByteArray &messageId); // virtual for unittest

private:
    LIBRUQOLACORE_NO_EXPORT void slotGetThreadMessagesDone(const QJsonObject &obj, const QByteArray &threadMessageId);
    LIBRUQOLACORE_NO_EXPORT void slotGetMessageDone(const QJsonObject &obj, const QByteArray &messageId);
    LIBRUQOLACORE_NO_EXPORT void slotGetSingleMessageDone(const QJsonObject &obj, const QByteArray &messageId);

    mutable QMap<QByteArray, RocketChatRestApi::GetThreadMessagesJob *> mThreadMessageJobs;
    QCache<QByteArray, ThreadMessageModel> mThreadMessageModels;

#ifdef USE_GET_MESSAGE_JOB
    mutable QMap<QByteArray, RocketChatRestApi::GetMessageJob *> mMessageJobs;
#else
    mutable QMap<QByteArray, RocketChatRestApi::MethodCallJob *> mMessageJobs;
#endif
    QCache<QByteArray, Message> mMessages;
    RocketChatAccount *const mRocketChatAccount;
};
