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

#include "discussion.h"
#include "utils.h"
#include <QDateTime>
#include <QJsonObject>
#include <KLocalizedString>

Discussion::Discussion()
{
}

QString Discussion::description() const
{
    return mDescription;
}

void Discussion::setDescription(const QString &description)
{
    mDescription = description;
}

QString Discussion::parentRoomId() const
{
    return mParentRoomId;
}

void Discussion::setParentRoomId(const QString &parentRoomId)
{
    mParentRoomId = parentRoomId;
}

int Discussion::numberMessages() const
{
    return mNumberMessages;
}

void Discussion::setNumberMessages(int numberMessages)
{
    mNumberMessages = numberMessages;
}

qint64 Discussion::lastMessage() const
{
    return mLastMessage;
}

void Discussion::setLastMessage(const qint64 &lastMessage)
{
    mLastMessage = lastMessage;
    mLastMessageDateTimeStr = QDateTime::fromMSecsSinceEpoch(mLastMessage).toString(Qt::SystemLocaleLongDate);
}

QString Discussion::timeStampDisplay() const
{
    return mTimeStampDateTimeStr;
}

QString Discussion::fname() const
{
    return mFname;
}

void Discussion::setFname(const QString &fname)
{
    mFname = fname;
}

QString Discussion::lastMessageDisplay() const
{
    return i18n("(Last Message: %1)", mLastMessageDateTimeStr);
}

qint64 Discussion::timeStamp() const
{
    return mTimeStamp;
}

void Discussion::setTimeStamp(const qint64 &timeStamp)
{
    mTimeStamp = timeStamp;
    mTimeStampDateTimeStr = QDateTime::fromMSecsSinceEpoch(mTimeStamp).toString(Qt::SystemLocaleLongDate);
}

QDebug operator <<(QDebug d, const Discussion &t)
{
    d << "Parent Id " << t.parentRoomId();
    d << "Last Message " << t.lastMessage();
    d << "Number of Messages " << t.numberMessages();
    d << "Description " << t.description();
    d << "Discussion Room Id " << t.discussionRoomId();
    d << "timestamp " << t.timeStamp();
    d << "fname " << t.fname();
    return d;
}

bool Discussion::operator ==(const Discussion &other) const
{
    return (description() == other.description())
           && (parentRoomId() == other.parentRoomId())
           && (numberMessages() == other.numberMessages())
           && (lastMessage() == other.lastMessage())
           && (discussionRoomId() == other.discussionRoomId())
           && (timeStamp() == other.timeStamp())
           && (fname() == other.fname());
}

Discussion &Discussion::operator=(const Discussion &other)
{
    mParentRoomId = other.parentRoomId();
    mDescription = other.description();
    mNumberMessages = other.numberMessages();
    mLastMessage = other.lastMessage();
    mDiscussionRoomId = other.discussionRoomId();
    mTimeStamp = other.timeStamp();
    mFname = other.fname();
    return *this;
}

void Discussion::parseDiscussion(const QJsonObject &o)
{
    mParentRoomId = o.value(QLatin1String("prid")).toString();
    mDescription = o.value(QLatin1String("description")).toString();
    mFname = o.value(QLatin1String("fname")).toString();
    mNumberMessages = o.value(QLatin1String("msgs")).toInt();
    mDiscussionRoomId = o.value(QLatin1String("_id")).toString();
    setLastMessage(Utils::parseIsoDate(QStringLiteral("lm"), o));
    setTimeStamp(Utils::parseIsoDate(QStringLiteral("ts"), o));
    //TODO autotranslate ??
}

QString Discussion::discussionRoomId() const
{
    return mDiscussionRoomId;
}

void Discussion::setDiscussionRoomId(const QString &discussionRoomId)
{
    mDiscussionRoomId = discussionRoomId;
}
