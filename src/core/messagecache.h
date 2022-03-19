/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
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

    Q_REQUIRED_RESULT ThreadMessageModel *threadMessageModel(const QString &threadMessageId);
    Q_REQUIRED_RESULT Message *messageForId(const QString &messageId);

Q_SIGNALS:
    void modelLoaded();
    void messageLoaded(const QString &threadMessageId);

protected:
    virtual bool startJob(RocketChatRestApi::RestApiAbstractJob *job); // virtual for unittest

private:
    void slotGetThreadMessagesDone(const QJsonObject &obj, const QString &threadMessageId);
    void slotGetMessageDone(const QJsonObject &obj, const QString &messageId);

    mutable QMap<QString, RocketChatRestApi::GetThreadMessagesJob *> mThreadMessageJobs;
    QCache<QString, ThreadMessageModel> mThreadMessageModels;

    mutable QMap<QString, RocketChatRestApi::GetMessageJob *> mMessageJobs;
    QCache<QString, Message> mMessages;
    RocketChatAccount *const mRocketChatAccount;
};
