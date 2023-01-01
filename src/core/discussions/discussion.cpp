/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "discussions/discussion.h"
#include "utils.h"
#include <KLocalizedString>
#include <QDateTime>
#include <QJsonObject>

Discussion::Discussion() = default;

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

void Discussion::setLastMessage(qint64 lastMessage)
{
    mLastMessage = lastMessage;
    QLocale l;
    mLastMessageDateTimeStr = l.toString(QDateTime::fromMSecsSinceEpoch(mLastMessage), QLocale::LongFormat);
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

const QString &Discussion::userName() const
{
    return mUserName;
}

void Discussion::setUserName(const QString &newUserName)
{
    mUserName = newUserName;
}

QString Discussion::lastMessageDisplay() const
{
    return i18n("(Last Message: %1)", mLastMessageDateTimeStr);
}

qint64 Discussion::timeStamp() const
{
    return mTimeStamp;
}

void Discussion::setTimeStamp(qint64 timeStamp)
{
    mTimeStamp = timeStamp;
    QLocale l;
    mTimeStampDateTimeStr = l.toString(QDateTime::fromMSecsSinceEpoch(mTimeStamp), QLocale::LongFormat);
}

QDebug operator<<(QDebug d, const Discussion &t)
{
    d << "Parent Id " << t.parentRoomId();
    d << "Last Message " << t.lastMessage();
    d << "Number of Messages " << t.numberMessages();
    d << "Description " << t.description();
    d << "Discussion Room Id " << t.discussionRoomId();
    d << "timestamp " << t.timeStamp();
    d << "fname " << t.fname();
    d << "mUserName " << t.userName();
    return d;
}

bool Discussion::operator==(const Discussion &other) const
{
    return (description() == other.description()) && (parentRoomId() == other.parentRoomId()) && (numberMessages() == other.numberMessages())
        && (lastMessage() == other.lastMessage()) && (discussionRoomId() == other.discussionRoomId()) && (timeStamp() == other.timeStamp())
        && (fname() == other.fname()) && (userName() == other.userName());
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
    const QJsonValue ownerValue = o.value(QLatin1String("u"));
    if (!ownerValue.isUndefined()) {
        const QJsonObject objOwner = ownerValue.toObject();
        mUserName = objOwner.value(QLatin1String("username")).toString();
    }
}

QString Discussion::discussionRoomId() const
{
    return mDiscussionRoomId;
}

void Discussion::setDiscussionRoomId(const QString &discussionRoomId)
{
    mDiscussionRoomId = discussionRoomId;
}
