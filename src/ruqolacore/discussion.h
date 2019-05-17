/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef DISCUSSION_H
#define DISCUSSION_H

#include "libruqola_private_export.h"
#include <QObject>
#include <QDebug>
class LIBRUQOLACORE_TESTS_EXPORT Discussion
{
    Q_GADGET
public:
    Discussion();

    Q_REQUIRED_RESULT QString description() const;
    void setDescription(const QString &description);

    Q_REQUIRED_RESULT QString parentRoomId() const;
    void setParentRoomId(const QString &parentRoomId);

    Q_REQUIRED_RESULT int numberMessages() const;
    void setNumberMessages(int numberMessages);

    Q_REQUIRED_RESULT qint64 lastMessage() const;
    void setLastMessage(const qint64 &lastMessage);

    Q_REQUIRED_RESULT bool operator ==(const Discussion &other) const;
    Discussion &operator =(const Discussion &other);

    void parseDiscussion(const QJsonObject &o);

    Q_REQUIRED_RESULT QString discussionRoomId() const;
    void setDiscussionRoomId(const QString &discussionRoomId);

private:
    QString mDescription;
    QString mParentRoomId;
    QString mDiscussionRoomId;
    int mNumberMessages = 0;
    qint64 mLastMessage = -1;
};
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Discussion &t);
Q_DECLARE_METATYPE(Discussion)
Q_DECLARE_TYPEINFO(Discussion, Q_MOVABLE_TYPE);
#endif // DISCUSSION_H
