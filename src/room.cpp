/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright 2017-2018 Laurent Montel <montel@kde.org>
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
#include "ruqola_debug.h"
#include "model/usersforroommodel.h"
#include "model/usersforroomfilterproxymodel.h"
#include "model/filesforroommodel.h"
#include "model/filesforroomfilterproxymodel.h"
#include "model/messagemodel.h"

#include <QJsonArray>
#include <QJsonDocument>

Room::Room(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
{
    mUsersModelForRoom = new UsersForRoomModel(this);
    mUsersModelForRoom->setObjectName(QStringLiteral("usersforroommodel"));
    mUsersModelForRoomProxyModel = new UsersForRoomFilterProxyModel(this);
    mUsersModelForRoomProxyModel->setObjectName(QStringLiteral("usersforroommodelproxymodel"));
    mUsersModelForRoomProxyModel->setSourceModel(mUsersModelForRoom);

    mFilesModelForRoom = new FilesForRoomModel(this);
    mFilesModelForRoom->setObjectName(QStringLiteral("filesmodelforrooms"));
    mFilesForRoomFilterProxyModel = new FilesForRoomFilterProxyModel(this);
    mFilesForRoomFilterProxyModel->setObjectName(QStringLiteral("filesforroomfiltermodelproxy"));
    mFilesForRoomFilterProxyModel->setSourceModel(mFilesModelForRoom);

    mMessageModel = new MessageModel(QString(), account, this);
}

bool Room::operator==(const Room &other) const
{
    //qDebug() << " other.id"<<other.id << " id " << id;
    return other.mRoomId == id();
}

bool Room::isEqual(const Room &other) const
{
    return (mRoomId == other.id())
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
           && (mOpen == other.open())
           && (mBlocker == other.blocker())
            && (mArchived == other.archived())
            && (mDescription == other.description());
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
    d << "blocker: " << t.blocker();
    d << "archived: " << t.archived();
    d << "description: " << t.description();
    return d;
}

void Room::parseUpdateRoom(const QJsonObject &json)
{
    qDebug() << " void Room::parseUpdateRoom(const QJsonObject &json)"<<json;
    if (json.contains(QLatin1String("_id"))) {
        setUserId(json.value(QLatin1String("_id")).toString());
    }
    if (json.contains(QLatin1String("rid"))) {
        setId(json.value(QLatin1String("rid")).toString());
    }
    if (json.contains(QLatin1String("alert"))) {
        setAlert(json[QStringLiteral("alert")].toBool());
    }
    if (json.contains(QLatin1String("f"))) {
        setFavorite(json[QStringLiteral("f")].toBool());
    }

    if (json.contains(QLatin1String("unread"))) {
        setUnread(json[QStringLiteral("unread")].toInt());
    }
    if (json.contains(QLatin1String("announcement"))) {
        setAnnouncement(json[QStringLiteral("announcement")].toString());
    }
    if (json.contains(QLatin1String("description"))) {
        setDescription(json[QStringLiteral("description")].toString());
    }
    if (json.contains(QLatin1String("open"))) {
        setOpen(json[QStringLiteral("open")].toBool());
    }
    if (json.contains(QLatin1String("topic"))) {
        setTopic(json[QStringLiteral("topic")].toString());
    }
    if (json.contains(QLatin1String("name"))) {
        setName(json[QStringLiteral("name")].toString());
    }
    if (json.contains(QLatin1String("name"))) {
        setName(json[QStringLiteral("name")].toString());
    }
    if (json.contains(QLatin1String("archived"))) {
        setArchived(json[QStringLiteral("archived")].toBool());
    } else {
        setArchived(false);
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
        Q_EMIT unreadChanged();
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

QStringList Room::mutedUsers() const
{
    return mMutedUsers;
}

void Room::setMutedUsers(const QStringList &mutedUsers)
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
        Q_EMIT userIdChanged();
    }
}

QString Room::id() const
{
    return mRoomId;
}

void Room::setId(const QString &id)
{
    if (mRoomId != id) {
        mRoomId = id;
        mMessageModel->setRoomID(id);
    }
}

bool Room::alert() const
{
    return mAlert;
}

void Room::setBlocker(bool block)
{
    if (mBlocker != block) {
        mBlocker = block;
        Q_EMIT blockerChanged();
    }
}

bool Room::blocker() const
{
    return mBlocker;
}

void Room::setAlert(bool alert)
{
    if (mAlert != alert) {
        mAlert = alert;
        Q_EMIT alertChanged();
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
        Q_EMIT readOnlyChanged();
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
    qDebug() << " void Room::parseRoom(const QJsonObject &json)"<<json;
    setUserId(json.value(QLatin1String("_id")).toString());
    setId(json.value(QLatin1String("rid")).toString());
    setName(json[QStringLiteral("name")].toString());
    setTopic(json[QStringLiteral("topic")].toString());
    setAnnouncement(json[QStringLiteral("announcement")].toString());
    setDescription(json[QStringLiteral("description")].toString());
    setReadOnly(json[QStringLiteral("ro")].toBool());
    const QJsonValue archivedValue = json.value(QLatin1String("archived"));
    if (!archivedValue.isUndefined()) {
        setArchived(archivedValue.toBool());
    }
    const QJsonValue blockerValue = json.value(QLatin1String("blocker"));
    if (!blockerValue.isUndefined()) {
        setBlocker(blockerValue.toBool());
    } else {
        setBlocker(false);
    }
}

void Room::parseSubscriptionRoom(const QJsonObject &json)
{
    const QString roomID = json.value(QLatin1String("rid")).toString();
    setId(roomID);
    setUserId(json.value(QLatin1String("_id")).toString());
    setName(json[QStringLiteral("name")].toString());
    setTopic(json[QStringLiteral("topic")].toString());
    setAnnouncement(json[QStringLiteral("announcement")].toString());
    setDescription(json[QStringLiteral("description")].toString());
    const QString roomType = json.value(QLatin1String("t")).toString();
    setChannelType(roomType);
    const QJsonValue favoriteValue = json.value(QLatin1String("f"));
    if (!favoriteValue.isUndefined()) {
        setFavorite(favoriteValue.toBool());
    }
    //Only private room has this settings.
    if (roomType == QLatin1String("p")) {
        setReadOnly(json[QStringLiteral("ro")].toBool());
    }

    setUnread(json[QStringLiteral("unread")].toInt());
    setOpen(json[QStringLiteral("open")].toBool());
    setAlert(json[QStringLiteral("alert")].toBool());
    const QJsonValue blockerValue = json.value(QLatin1String("blocker"));
    if (!blockerValue.isUndefined()) {
        setBlocker(blockerValue.toBool());
    } else {
        setBlocker(false);
    }
    const QJsonValue archivedValue = json.value(QLatin1String("archived"));
    if (!archivedValue.isUndefined()) {
        setArchived(archivedValue.toBool());
    } else {
        setArchived(false);
    }

    const QJsonArray mutedArray = json.value(QLatin1String("muted")).toArray();
    QStringList lst;
    lst.reserve(mutedArray.count());
    for (int i = 0; i < mutedArray.count(); ++i) {
        lst << mutedArray.at(i).toString();
    }

    setMutedUsers(lst);
    //TODO add muted
}

Room *Room::fromJSon(const QJsonObject &o)
{
    //FIXME
    Room *r = new Room(nullptr);

    r->setId(o[QStringLiteral("rid")].toString());
    r->setChannelType(o[QStringLiteral("t")].toString());
    r->setName(o[QStringLiteral("name")].toString());
    r->setUserName(o[QStringLiteral("userName")].toString());
    r->setUserId(o[QStringLiteral("userID")].toString());
    r->setTopic(o[QStringLiteral("topic")].toString());
    r->setJitsiTimeout(o[QStringLiteral("jitsiTimeout")].toDouble());
    r->setReadOnly(o[QStringLiteral("ro")].toBool());
    r->setUnread(o[QStringLiteral("unread")].toInt(0));
    r->setAnnouncement(o[QStringLiteral("announcement")].toString());
    r->setSelected(o[QStringLiteral("selected")].toBool());
    r->setFavorite(o[QStringLiteral("favorite")].toBool());
    r->setAlert(o[QStringLiteral("alert")].toBool());
    r->setOpen(o[QStringLiteral("open")].toBool());
    r->setArchived(o[QStringLiteral("archived")].toBool());
    r->setDescription(o[QStringLiteral("description")].toString());
    //TODO ???
    r->setBlocker(o[QStringLiteral("blocker")].toBool());
    const QJsonArray mutedArray = o.value(QLatin1String("mutedUsers")).toArray();
    QStringList lst;
    lst.reserve(mutedArray.count());
    for (int i = 0; i < mutedArray.count(); ++i) {
        lst <<mutedArray.at(i).toString();
    }
    r->setMutedUsers(lst);

    return r;
}

QByteArray Room::serialize(Room *r)
{
    QJsonDocument d;
    QJsonObject o;

    o[QStringLiteral("rid")] = r->id();
    o[QStringLiteral("t")] = r->channelType();
    o[QStringLiteral("name")] = r->name();
    o[QStringLiteral("userName")] = r->userName();
    o[QStringLiteral("userID")] = r->userId();
    o[QStringLiteral("topic")] = r->topic();
    o[QStringLiteral("jitsiTimeout")] = r->jitsiTimeout();
    o[QStringLiteral("ro")] = r->readOnly();
    o[QStringLiteral("unread")] = r->unread();
    o[QStringLiteral("announcement")] = r->announcement();
    o[QStringLiteral("selected")] = r->selected();
    o[QStringLiteral("favorite")] = r->favorite();
    o[QStringLiteral("alert")] = r->alert();
    o[QStringLiteral("open")] = r->open();
    o[QStringLiteral("blocker")] = r->blocker();
    o[QStringLiteral("archived")] = r->archived();
    o[QStringLiteral("description")] = r->description();

    //Urls
    if (!r->mutedUsers().isEmpty()) {
        QJsonArray array;
        const int nbMuted{
            r->mutedUsers().count()
        };
        for (int i = 0; i < nbMuted; ++i) {
            array.append(r->mutedUsers().at(i));
        }
        o[QStringLiteral("mutedUsers")] = array;
    }

    d.setObject(o);
    return d.toBinaryData();
}

UsersForRoomModel *Room::usersModelForRoom() const
{
    return mUsersModelForRoom;
}

UsersForRoomFilterProxyModel *Room::usersModelForRoomProxyModel() const
{
    return mUsersModelForRoomProxyModel;
}

FilesForRoomModel *Room::filesModelForRoom() const
{
    return mFilesModelForRoom;
}

FilesForRoomFilterProxyModel *Room::filesForRoomFilterProxyModel() const
{
    return mFilesForRoomFilterProxyModel;
}

MessageModel *Room::messageModel() const
{
    return mMessageModel;
}

QString Room::inputMessage() const
{
    return mInputMessage;
}

void Room::setInputMessage(const QString &inputMessage)
{
    mInputMessage = inputMessage;
}

bool Room::archived() const
{
    return mArchived;
}

void Room::setArchived(bool archived)
{
    if (mArchived != archived) {
        mArchived = archived;
        Q_EMIT archivedChanged();
    }
}

QString Room::description() const
{
    return mDescription;
}

void Room::setDescription(const QString &description)
{
    if (mDescription != description) {
        mDescription = description;
        Q_EMIT descriptionChanged();
    }
}
