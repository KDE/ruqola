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

Room::Room(QObject *parent)
    : QObject(parent)
{
}

bool Room::operator==(const Room &other) const
{
    //qDebug() << " other.id"<<other.id << " id " << id;
    return other.mId == id();
}

bool Room::isEqual(const Room &other) const
{
    return (mId == other.id())
           && (mChannelType == other.channelType())
           && (mName == other.name())
           && (mAnnouncement == other.announcement())
           && (mUserName == other.userName())
           && (mUserId == other.userId())
           && (mTopic == other.topic())
           && (mMutedUsers == other.mutedUsers())
           && (mJitsiTimeout == other.jitsiTimeout())
           && (mReadOnly == other.readOnly())
           && (mUnread == other.unread())
           && (mSelected == other.selected())
           && (mFavorite == other.favorite())
           && (mOpen == other.open());
}

QString Room::name() const
{
    return mName;
}

QDebug operator <<(QDebug d, const Room &t)
{
    d << "id :" << t.id();
    d << "type :" << t.channelType();
    d << "name :" << t.name();
    d << "mAnnouncement :" << t.announcement();
    d << "userName :" << t.userName();
    d << "userID :" << t.userId();
    d << "topic :" << t.topic();
    d << "mutedUsers :" << t.mutedUsers();
    d << "jitsiTimeout :" << t.jitsiTimeout();
    d << "ro :" << t.readOnly();
    d << "unread :" << t.unread();
    d << "selected :" << t.selected();
    d << "favorite :" << t.favorite();
    d << "open :" << t.open();
    return d;
}

void Room::parseUpdateRoom(const QJsonObject &json)
{
    //QJsonArray(["updated",{"_id":"7jHqcrZ8FYXJBwgRB","_updatedAt":{"$date":1503902695955},"alert":false,"f":true,"groupMentions":0,"ls":{"$date":1503902695955},"name":"dev","open":true,"rid":"dBWXYy4nyBHn8Q7dv","t":"c","ts":{"$date":1493034182680},"u":{"_id":"uKK39zoewTkdacidH","username":"laurent"},"unread":0,"userMentions":0}])
    if (json.contains(QStringLiteral("rid"))) {
        setId(json.value(QStringLiteral("rid")).toString());
    }
    if (json.contains(QStringLiteral("alert"))) {
        setAlert(json[QStringLiteral("alert")].toBool());
    }
    if (json.contains(QStringLiteral("f"))) {
        //TODO
    }

    if (json.contains(QStringLiteral("unread"))) {
        setUnread(json[QStringLiteral("unread")].toInt());
    }
    if (json.contains(QStringLiteral("announcement"))) {
        setAnnouncement(json[QStringLiteral("announcement")].toString());
    }
    if (json.contains(QStringLiteral("open"))) {
        setOpen(json[QStringLiteral("open")].toBool());
    }
    if (json.contains(QStringLiteral("topic"))) {
        setTopic(json[QStringLiteral("topic")].toString());
    }
}

bool Room::selected() const
{
    return mSelected;
}

void Room::setSelected(bool selected)
{
    if (mSelected != selected) {
        mSelected = selected;
    }
}

int Room::unread() const
{
    return mUnread;
}

void Room::setUnread(int unread)
{
    if (mUnread != unread) {
        mUnread = unread;
    }
}

qint64 Room::jitsiTimeout() const
{
    return mJitsiTimeout;
}

void Room::setJitsiTimeout(const qint64 &jitsiTimeout)
{
    if (mJitsiTimeout != jitsiTimeout) {
        mJitsiTimeout = jitsiTimeout;
    }
}

QString Room::mutedUsers() const
{
    return mMutedUsers;
}

void Room::setMutedUsers(const QString &mutedUsers)
{
    if (mMutedUsers != mutedUsers) {
        mMutedUsers = mutedUsers;
    }
}

QString Room::userId() const
{
    return mUserId;
}

void Room::setUserId(const QString &userId)
{
    if (mUserId != userId) {
        mUserId = userId;
    }
}

QString Room::userName() const
{
    return mUserName;
}

void Room::setUserName(const QString &userName)
{
    if (mUserName != userName) {
        mUserName = userName;
    }
}

QString Room::id() const
{
    return mId;
}

void Room::setId(const QString &id)
{
    if (mId != id) {
        mId = id;
    }
}

bool Room::alert() const
{
    return mAlert;
}

void Room::setAlert(bool alert)
{
    if (mAlert != alert) {
        mAlert = alert;
    }
}

bool Room::open() const
{
    return mOpen;
}

void Room::setOpen(bool open)
{
    if (mOpen != open) {
        mOpen = open;
    }
}

bool Room::readOnly() const
{
    return mReadOnly;
}

void Room::setReadOnly(bool readOnly)
{
    if (mReadOnly != readOnly) {
        mReadOnly = readOnly;
    }
}

QString Room::topic() const
{
    return mTopic;
}

void Room::setTopic(const QString &topic)
{
    if (mTopic != topic) {
        mTopic = topic;
        Q_EMIT topicChanged();
    }
}

bool Room::favorite() const
{
    return mFavorite;
}

void Room::setFavorite(bool favorite)
{
    if (mFavorite != favorite) {
        mFavorite = favorite;
        Q_EMIT favoriteChanged();
    }
}

QString Room::channelType() const
{
    return mChannelType;
}

void Room::setChannelType(const QString &channelType)
{
    if (mChannelType != channelType) {
        mChannelType = channelType;
    }
}

QString Room::announcement() const
{
    return mAnnouncement;
}

void Room::setAnnouncement(const QString &announcement)
{
    if (mAnnouncement != announcement) {
        mAnnouncement = announcement;
        Q_EMIT announcementChanged();
    }
}

void Room::setName(const QString &name)
{
    if (mName != name) {
        mName = name;
        Q_EMIT nameChanged();
    }
}

void Room::parseRoom(const QJsonObject &json)
{
    setId(json.value(QStringLiteral("_id")).toString());
    setName(json[QStringLiteral("name")].toString());
    setTopic(json[QStringLiteral("topic")].toString());
    setAnnouncement(json[QStringLiteral("announcement")].toString());
}

void Room::parseSubscriptionRoom(const QJsonObject &json)
{
    const QString roomID = json.value(QStringLiteral("rid")).toString();
    setId(roomID);
    setName(json[QStringLiteral("name")].toString());
    setTopic(json[QStringLiteral("topic")].toString());
    setAnnouncement(json[QStringLiteral("announcement")].toString());
    const QString roomType = json.value(QStringLiteral("t")).toString();
    setChannelType(roomType);
    QJsonValue favoriteValue = json.value(QStringLiteral("f"));
    if (!favoriteValue.isUndefined()) {
        setFavorite(favoriteValue.toBool());
    }
    //Only private room has this settings.
    if (roomType == QLatin1String("p")) {
        setReadOnly(json[QStringLiteral("ro")].toString() == QLatin1String("true"));
    }
    setUnread(json[QStringLiteral("unread")].toInt());
    setOpen(json[QStringLiteral("open")].toBool());
    setAlert(json[QStringLiteral("alert")].toBool());
}
