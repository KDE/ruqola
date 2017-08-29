/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright 2017  Laurent Montel <montel@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "room.h"
#include <QDebug>
Room::Room()
{
}

bool Room::operator==(const Room &other) const
{
    //qDebug() << " other.id"<<other.id << " id " << id;
    return other.id == id;
}

bool Room::operator<(const Room &other) const
{
    return mName < other.mName;
}

bool Room::isEqual(const Room &other) const
{
    return (id == other.id)
           && (mChannelType == other.mChannelType)
           && (mName == other.mName)
           && (mAnnouncement == other.mAnnouncement)
           && (mUserName == other.mUserName)
           && (mUserId == other.mUserId)
           && (mTopic == other.mTopic)
           && (mMutedUsers == other.mMutedUsers)
           && (mJitsiTimeout == other.mJitsiTimeout)
           && (mReadOnly == other.mReadOnly)
           && (mUnread == other.mUnread)
           && (mSelected == other.mSelected)
           && (mFavorite == other.mFavorite)
           && (mOpen == other.mOpen);
}

QDebug operator <<(QDebug d, const Room &t)
{
    d << "id :" << t.id;
    d << "type :" << t.mChannelType;
    d << "name :" << t.mName;
    d << "mAnnouncement :" << t.mAnnouncement;
    d << "userName :" << t.mUserName;
    d << "userID :" << t.mUserId;
    d << "topic :" << t.mTopic;
    d << "mutedUsers :" << t.mMutedUsers;
    d << "jitsiTimeout :" << t.mJitsiTimeout;
    d << "ro :" << t.mReadOnly;
    d << "unread :" << t.mUnread;
    d << "selected :" << t.mSelected;
    d << "favorite :" << t.mFavorite;
    d << "open :" << t.mOpen;
    return d;
}

void Room::parseUpdateRoom(const QJsonObject &json)
{
    //QJsonArray(["updated",{"_id":"7jHqcrZ8FYXJBwgRB","_updatedAt":{"$date":1503902695955},"alert":false,"f":true,"groupMentions":0,"ls":{"$date":1503902695955},"name":"dev","open":true,"rid":"dBWXYy4nyBHn8Q7dv","t":"c","ts":{"$date":1493034182680},"u":{"_id":"uKK39zoewTkdacidH","username":"laurent"},"unread":0,"userMentions":0}])
    if (json.contains(QStringLiteral("rid"))) {
        id = json.value(QStringLiteral("rid")).toString();
    }
    if (json.contains(QStringLiteral("alert"))) {
        mAlert = json[QStringLiteral("alert")].toBool();
    }
    if (json.contains(QStringLiteral("f"))) {
    }
    if (json.contains(QStringLiteral("unread"))) {
        mUnread = json[QStringLiteral("unread")].toInt();
    }
    if (json.contains(QStringLiteral("announcement"))) {
        mAnnouncement = json[QStringLiteral("announcement")].toString();
    }
    if (json.contains(QStringLiteral("open"))) {
        mOpen = json[QStringLiteral("open")].toBool();
    }
    if (json.contains(QStringLiteral("topic"))) {
        mTopic = json[QStringLiteral("topic")].toString();
    }
}

void Room::parseRoom(const QJsonObject &json)
{
    id = json.value(QStringLiteral("_id")).toString();
    mName = json[QStringLiteral("name")].toString();
    mTopic = json[QStringLiteral("topic")].toString();
    mAnnouncement = json[QStringLiteral("announcement")].toString();
}

void Room::parseSubscriptionRoom(const QJsonObject &json)
{
    const QString roomID = json.value(QStringLiteral("rid")).toString();
    id = roomID;
    mName = json[QStringLiteral("name")].toString();
    mTopic = json[QStringLiteral("topic")].toString();
    mAnnouncement = json[QStringLiteral("announcement")].toString();
    const QString roomType = json.value(QStringLiteral("t")).toString();
    mChannelType = roomType;
    QJsonValue favoriteValue = json.value(QStringLiteral("f"));
    if (!favoriteValue.isUndefined()) {
        mFavorite = favoriteValue.toBool();
    }
    //Only private room has this settings.
    if (roomType == QLatin1String("p")) {
        mReadOnly = json[QStringLiteral("ro")].toString() == QLatin1String("true");
    }
    mUnread = json[QStringLiteral("unread")].toInt();
    mOpen = json[QStringLiteral("open")].toBool();
    mAlert = json[QStringLiteral("alert")].toBool();
}
