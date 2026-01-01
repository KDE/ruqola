/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QMetaType>

class LIBRUQOLACORE_TESTS_EXPORT Discussion
{
    Q_GADGET
public:
    Discussion();
    Discussion(const Discussion &other) = default;

    [[nodiscard]] QString description() const;
    void setDescription(const QString &description);

    [[nodiscard]] QByteArray parentRoomId() const;
    void setParentRoomId(const QByteArray &parentRoomId);

    [[nodiscard]] int numberMessages() const;
    void setNumberMessages(int numberMessages);

    [[nodiscard]] qint64 lastMessage() const;
    void setLastMessage(qint64 lastMessage);

    [[nodiscard]] bool operator==(const Discussion &other) const;
    Discussion &operator=(const Discussion &other) = default;

    void parseDiscussion(const QJsonObject &o);

    [[nodiscard]] QByteArray discussionRoomId() const;
    void setDiscussionRoomId(const QByteArray &discussionRoomId);

    [[nodiscard]] QString lastMessageDisplay() const;

    [[nodiscard]] qint64 timeStamp() const;
    void setTimeStamp(qint64 timeStamp);

    [[nodiscard]] QString timeStampDisplay() const;

    [[nodiscard]] QString fname() const;
    void setFname(const QString &fname);

    [[nodiscard]] const QString &userName() const;
    void setUserName(const QString &newUserName);

private:
    QString mDescription;
    QByteArray mParentRoomId;
    QByteArray mDiscussionRoomId;
    QString mLastMessageDateTimeStr;
    QString mTimeStampDateTimeStr;
    QString mFname;
    QString mUserName;
    int mNumberMessages = 0;
    qint64 mLastMessage = -1;
    qint64 mTimeStamp = -1;
};
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Discussion &t);
Q_DECLARE_TYPEINFO(Discussion, Q_RELOCATABLE_TYPE);
