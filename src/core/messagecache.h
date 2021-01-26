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

#ifndef MESSAGECACHE_H
#define MESSAGECACHE_H

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
    explicit MessageCache(QObject *parent = nullptr);
    ~MessageCache() override;

    ThreadMessageModel *threadMessageModel(const QString &threadMessageId);
    Message *messageForId(const QString &messageId);

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
};

#endif // MESSAGECACHE_H
