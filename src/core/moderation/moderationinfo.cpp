/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationinfo.h"
#include "utils.h"

ModerationInfo::ModerationInfo() = default;

QDebug operator<<(QDebug d, const ModerationInfo &t)
{
    d.space() << "userId" << t.userId();
    d.space() << "name" << t.name();
    d.space() << "username" << t.userName();
    d.space() << "msgId" << t.msgId();
    d.space() << "mCount" << t.count();
    d.space() << "mIsUserDeleted" << t.isUserDeleted();
    d.space() << "mMessage" << t.message();
    d.space() << "mCreatedAt" << t.createdAt();
    d.space() << "mRoomName" << t.roomList();
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

void ModerationInfo::parseRoomList(const QJsonArray &rooms)
{
    for (int i = 0; i < rooms.size(); i++) {
        const QJsonObject o = rooms.at(i).toObject();
        const QString fname = o[QLatin1StringView("fname")].toString();
        const QString name = o[QLatin1StringView("name")].toString();
        mRoomList.append(fname.isEmpty() ? name : fname);
    }
}

void ModerationInfo::parseModerationInfo(const QJsonObject &o)
{
    // qDebug() << " ModerationInfo " << o;
    mUserId = o[QLatin1StringView("userId")].toString().toLatin1();
    mName = o[QLatin1StringView("name")].toString();
    mUserName = o[QLatin1StringView("username")].toString();
    mMsgId = o[QLatin1StringView("msgId")].toString().toLatin1();
    mCount = o[QLatin1StringView("count")].toInt();
    mIsUserDeleted = o[QLatin1StringView("isUserDeleted")].toBool();
    mMessage = o[QLatin1StringView("message")].toString();
    setCreatedAt(Utils::parseIsoDate(QStringLiteral("ts"), o));
    parseRoomList(o[QLatin1StringView("rooms")].toArray());
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

QByteArray ModerationInfo::userId() const
{
    return mUserId;
}

void ModerationInfo::setUserId(const QByteArray &newUserId)
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

QByteArray ModerationInfo::msgId() const
{
    return mMsgId;
}

void ModerationInfo::setMsgId(const QByteArray &newMsgId)
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
