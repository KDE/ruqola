/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
class LIBRUQOLACORE_TESTS_EXPORT Discussion
{
    Q_GADGET
public:
    Discussion();
    Discussion(const Discussion &other) = default;

    Q_REQUIRED_RESULT QString description() const;
    void setDescription(const QString &description);

    Q_REQUIRED_RESULT QString parentRoomId() const;
    void setParentRoomId(const QString &parentRoomId);

    Q_REQUIRED_RESULT int numberMessages() const;
    void setNumberMessages(int numberMessages);

    Q_REQUIRED_RESULT qint64 lastMessage() const;
    void setLastMessage(qint64 lastMessage);

    Q_REQUIRED_RESULT bool operator==(const Discussion &other) const;
    Discussion &operator=(const Discussion &other) = default;

    void parseDiscussion(const QJsonObject &o);

    Q_REQUIRED_RESULT QString discussionRoomId() const;
    void setDiscussionRoomId(const QString &discussionRoomId);

    Q_REQUIRED_RESULT QString lastMessageDisplay() const;

    Q_REQUIRED_RESULT qint64 timeStamp() const;
    void setTimeStamp(qint64 timeStamp);

    Q_REQUIRED_RESULT QString timeStampDisplay() const;

    Q_REQUIRED_RESULT QString fname() const;
    void setFname(const QString &fname);

    Q_REQUIRED_RESULT const QString &userName() const;
    void setUserName(const QString &newUserName);

private:
    QString mDescription;
    QString mParentRoomId;
    QString mDiscussionRoomId;
    QString mLastMessageDateTimeStr;
    QString mTimeStampDateTimeStr;
    QString mFname;
    QString mUserName;
    int mNumberMessages = 0;
    qint64 mLastMessage = -1;
    qint64 mTimeStamp = -1;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Discussion &t);
Q_DECLARE_METATYPE(Discussion)
Q_DECLARE_TYPEINFO(Discussion, Q_MOVABLE_TYPE);
