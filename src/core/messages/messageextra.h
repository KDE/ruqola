/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSharedData>
#include <QString>

class LIBRUQOLACORE_EXPORT MessageExtra : public QSharedData
{
public:
    MessageExtra();
    ~MessageExtra();

    [[nodiscard]] QString localTranslation() const;
    void setLocalTranslation(const QString &newLocalTranslation);

    [[nodiscard]] int threadCount() const;
    void setThreadCount(int threadCount);

    [[nodiscard]] int discussionCount() const;
    void setDiscussionCount(int discussionCount);

    [[nodiscard]] QByteArray discussionRoomId() const;
    void setDiscussionRoomId(const QByteArray &discussionRoomId);

    [[nodiscard]] QByteArray threadMessageId() const;
    void setThreadMessageId(const QByteArray &threadMessageId);

    [[nodiscard]] qint64 threadLastMessage() const;
    void setThreadLastMessage(qint64 newThreadLastMessage);

    [[nodiscard]] qint64 discussionLastMessage() const;
    void setDiscussionLastMessage(qint64 newDiscussionLastMessage);

private:
    // Local translation
    QString mLocalTranslation;

    // drid discussion room id
    QByteArray mDiscussionRoomId;

    // tmid
    QByteArray mThreadMessageId;

    // Thread last message
    qint64 mThreadLastMessage = -1;
    // Thread last message
    qint64 mDiscussionLastMessage = -1;

    // Thread count
    int mThreadCount = 0;

    // Thread count
    int mDiscussionCount = 0;
};
