/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportedmessageinfo.h"
#include "utils.h"

ModerationReportedMessageInfo::ModerationReportedMessageInfo() = default;

QDebug operator<<(QDebug d, const ModerationReportedMessageInfo &t)
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
bool ModerationReportedMessageInfo::operator==(const ModerationReportedMessageInfo &other) const
{
    return mUserId == other.mUserId && mName == other.mName && mUserName == other.mUserName && mUserId == other.mUserId && mCount == other.mCount
        && mIsUserDeleted == other.mIsUserDeleted && mMessage == other.mMessage && mCreatedAt == other.mCreatedAt && mRoomList == other.mRoomList;
}

void ModerationReportedMessageInfo::parseRoomList(const QJsonArray &rooms)
{
    for (int i = 0; i < rooms.size(); i++) {
        const QJsonObject o = rooms.at(i).toObject();
        const QString fname = o[QLatin1StringView("fname")].toString();
        const QString name = o[QLatin1StringView("name")].toString();
        mRoomList.append(fname.isEmpty() ? name : fname);
    }
}

void ModerationReportedMessageInfo::parseModerationInfo(const QJsonObject &o)
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

void ModerationReportedMessageInfo::setCreatedAt(qint64 newCreatedAt)
{
    mCreatedAt = newCreatedAt;
    if (mCreatedAt != -1) {
        QLocale l;
        mCreateAtDisplayDateTime = l.toString(QDateTime::fromMSecsSinceEpoch(mCreatedAt), QLocale::LongFormat);
    }
}

QStringList ModerationReportedMessageInfo::roomList() const
{
    return mRoomList;
}

void ModerationReportedMessageInfo::setRoomList(const QStringList &newRoomName)
{
    mRoomList = newRoomName;
}

qint64 ModerationReportedMessageInfo::createdAt() const
{
    return mCreatedAt;
}

const QString &ModerationReportedMessageInfo::createAtDisplayDateTime() const
{
    return mCreateAtDisplayDateTime;
}

QByteArray ModerationReportedMessageInfo::userId() const
{
    return mUserId;
}

void ModerationReportedMessageInfo::setUserId(const QByteArray &newUserId)
{
    mUserId = newUserId;
}

QString ModerationReportedMessageInfo::name() const
{
    return mName;
}

void ModerationReportedMessageInfo::setName(const QString &newName)
{
    mName = newName;
}

QString ModerationReportedMessageInfo::userName() const
{
    return mUserName;
}

void ModerationReportedMessageInfo::setUserName(const QString &newUserName)
{
    mUserName = newUserName;
}

QByteArray ModerationReportedMessageInfo::msgId() const
{
    return mMsgId;
}

void ModerationReportedMessageInfo::setMsgId(const QByteArray &newMsgId)
{
    mMsgId = newMsgId;
}

int ModerationReportedMessageInfo::count() const
{
    return mCount;
}

void ModerationReportedMessageInfo::setCount(int newCount)
{
    mCount = newCount;
}

bool ModerationReportedMessageInfo::isUserDeleted() const
{
    return mIsUserDeleted;
}

void ModerationReportedMessageInfo::setIsUserDeleted(bool newIsUserDeleted)
{
    mIsUserDeleted = newIsUserDeleted;
}

QString ModerationReportedMessageInfo::message() const
{
    return mMessage;
}

void ModerationReportedMessageInfo::setMessage(const QString &newMessage)
{
    mMessage = newMessage;
}
