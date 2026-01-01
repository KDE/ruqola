/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "discussions/discussion.h"

#include "utils.h"
#include <KLocalizedString>
#include <QDateTime>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
Discussion::Discussion() = default;

QString Discussion::description() const
{
    return mDescription;
}

void Discussion::setDescription(const QString &description)
{
    mDescription = description;
}

QByteArray Discussion::parentRoomId() const
{
    return mParentRoomId;
}

void Discussion::setParentRoomId(const QByteArray &parentRoomId)
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
    d.space() << "Parent Id" << t.parentRoomId();
    d.space() << "Last Message" << t.lastMessage();
    d.space() << "Number of Messages" << t.numberMessages();
    d.space() << "Description" << t.description();
    d.space() << "Discussion Room Id" << t.discussionRoomId();
    d.space() << "timestamp" << t.timeStamp();
    d.space() << "fname" << t.fname();
    d.space() << "mUserName" << t.userName();
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
    mParentRoomId = o.value("prid"_L1).toString().toLatin1();
    mDescription = o.value("description"_L1).toString();
    mFname = o.value("fname"_L1).toString();
    mNumberMessages = o.value("msgs"_L1).toInt();
    mDiscussionRoomId = o.value("_id"_L1).toString().toLatin1();
    setLastMessage(Utils::parseIsoDate(u"lm"_s, o));
    setTimeStamp(Utils::parseIsoDate(u"ts"_s, o));
    const QJsonValue ownerValue = o.value("u"_L1);
    if (!ownerValue.isUndefined()) {
        const QJsonObject objOwner = ownerValue.toObject();
        mUserName = objOwner.value("username"_L1).toString();
    }
}

QByteArray Discussion::discussionRoomId() const
{
    return mDiscussionRoomId;
}

void Discussion::setDiscussionRoomId(const QByteArray &discussionRoomId)
{
    mDiscussionRoomId = discussionRoomId;
}

#include "moc_discussion.cpp"
