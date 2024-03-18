/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageextra.h"

MessageExtra::MessageExtra() = default;

QString MessageExtra::localTranslation() const
{
    return mLocalTranslation;
}

void MessageExtra::setLocalTranslation(const QString &newLocalTranslation)
{
    mLocalTranslation = newLocalTranslation;
}

QByteArray MessageExtra::threadMessageId() const
{
    return mThreadMessageId;
}

void MessageExtra::setThreadMessageId(const QByteArray &threadMessageId)
{
    mThreadMessageId = threadMessageId;
}

QByteArray MessageExtra::discussionRoomId() const
{
    return mDiscussionRoomId;
}

void MessageExtra::setDiscussionRoomId(const QByteArray &discussionRoomId)
{
    mDiscussionRoomId = discussionRoomId;
}

int MessageExtra::discussionCount() const
{
    return mDiscussionCount;
}

void MessageExtra::setDiscussionCount(int discussionCount)
{
    mDiscussionCount = discussionCount;
}

int MessageExtra::threadCount() const
{
    return mThreadCount;
}

void MessageExtra::setThreadCount(int threadCount)
{
    mThreadCount = threadCount;
}
