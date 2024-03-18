/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QSharedData>
#include <QString>

class LIBRUQOLACORE_TESTS_EXPORT MessageExtra : public QSharedData
{
public:
    MessageExtra();

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

private:
    // Local translation
    QString mLocalTranslation;

    // drid discussion room id
    QByteArray mDiscussionRoomId;

    // tmid
    QByteArray mThreadMessageId;

    // Thread count
    int mThreadCount = 0;

    // Thread count
    int mDiscussionCount = 0;
};
