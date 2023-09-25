/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationinfo.h"
#include "utils.h"

ModerationInfo::ModerationInfo() = default;

QDebug operator<<(QDebug d, const ModerationInfo &t)
{
    d << "userId " << t.userId();
    d << "name " << t.name();
    d << "username " << t.userName();
    d << "msgId " << t.msgId();
    d << "mCount " << t.count();
    d << "mIsUserDeleted " << t.isUserDeleted();
    d << "mMessage " << t.message();
    d << "mCreatedAt " << t.createdAt();
    d << "mRoomName " << t.roomList();
    return d;
}

// obj QJsonObject({"count":1,"offset":0,"reports":[{"count":1,"isUserDeleted":false,"message":"Fghd","msgId":"eJ443teFnx7hTG5pZ","name":"Laurent m",
// "rooms":[{"_id":"GotJhd87jLScanhwr","fname":"test4","name":"test4","t":"c"}],"ts":"2023-09-20T15:09:37.959Z","userId":"dddd","username":"laurent"}],
// "success":true,"total":1})
bool ModerationInfo::operator==(const ModerationInfo &other) const
{
    return mUserId == other.mUserId && mName == other.mName && mUserName == other.mUserName && mUserId == other.mUserId && mCount == other.mCount
        && mIsUserDeleted == other.mIsUserDeleted && mMessage == other.mMessage && mCreatedAt == other.mCreatedAt && mRoomList == other.mRoomList;
}

void ModerationInfo::parseRoom()
{
    // TODO
}

void ModerationInfo::parseModerationInfo(const QJsonObject &o)
{
    mUserId = o[QLatin1String("userId")].toString();
    mName = o[QLatin1String("name")].toString();
    mUserName = o[QLatin1String("username")].toString();
    mMsgId = o[QLatin1String("msgId")].toString();
    mCount = o[QLatin1String("count")].toInt();
    mIsUserDeleted = o[QLatin1String("isUserDeleted")].toBool();
    mMessage = o[QLatin1String("message")].toString();
    setCreatedAt(Utils::parseIsoDate(QStringLiteral("ts"), o));
    // TODO
}

void ModerationInfo::setCreatedAt(qint64 newCreatedAt)
{
    mCreatedAt = newCreatedAt;
    if (mCreatedAt != -1) {
        QLocale l;
        mCreateAtDisplayDateTime = l.toString(QDateTime::fromMSecsSinceEpoch(mCreatedAt), QLocale::LongFormat);
    }
}

QStringList ModerationInfo::roomList() const
{
    return mRoomList;
}

void ModerationInfo::setRoomList(const QStringList &newRoomName)
{
    mRoomList = newRoomName;
}

qint64 ModerationInfo::createdAt() const
{
    return mCreatedAt;
}

const QString &ModerationInfo::createAtDisplayDateTime() const
{
    return mCreateAtDisplayDateTime;
}

QString ModerationInfo::userId() const
{
    return mUserId;
}

void ModerationInfo::setUserId(const QString &newUserId)
{
    mUserId = newUserId;
}

QString ModerationInfo::name() const
{
    return mName;
}

void ModerationInfo::setName(const QString &newName)
{
    mName = newName;
}

QString ModerationInfo::userName() const
{
    return mUserName;
}

void ModerationInfo::setUserName(const QString &newUserName)
{
    mUserName = newUserName;
}

QString ModerationInfo::msgId() const
{
    return mMsgId;
}

void ModerationInfo::setMsgId(const QString &newMsgId)
{
    mMsgId = newMsgId;
}

int ModerationInfo::count() const
{
    return mCount;
}

void ModerationInfo::setCount(int newCount)
{
    mCount = newCount;
}

bool ModerationInfo::isUserDeleted() const
{
    return mIsUserDeleted;
}

void ModerationInfo::setIsUserDeleted(bool newIsUserDeleted)
{
    mIsUserDeleted = newIsUserDeleted;
}

QString ModerationInfo::message() const
{
    return mMessage;
}

void ModerationInfo::setMessage(const QString &newMessage)
{
    mMessage = newMessage;
}
