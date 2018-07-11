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

#include "rocketchataccount.h"
#include "room.h"
#include "utils.h"
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
    , mRocketChatAccount(account)
{
    mUsersModelForRoom = new UsersForRoomModel(this);
    mUsersModelForRoom->setObjectName(QStringLiteral("usersforroommodel"));
    mUsersModelForRoomProxyModel = new UsersForRoomFilterProxyModel(this);
    mUsersModelForRoomProxyModel->setObjectName(QStringLiteral("usersforroommodelproxymodel"));
    mUsersModelForRoomProxyModel->setSourceModel(mUsersModelForRoom);

    mFilesModelForRoom = new FilesForRoomModel(mRocketChatAccount, this);
    mFilesModelForRoom->setObjectName(QStringLiteral("filesmodelforrooms"));
    mFilesForRoomFilterProxyModel = new FilesForRoomFilterProxyModel(this);
    mFilesForRoomFilterProxyModel->setObjectName(QStringLiteral("filesforroomfiltermodelproxy"));
    mFilesForRoomFilterProxyModel->setSourceModel(mFilesModelForRoom);

    mMessageModel = new MessageModel(QString(), mRocketChatAccount, this);
}

bool Room::operator==(const Room &other) const
{
    //qDebug() << " other.id"<<other.id << " id " << id;
    return other.mRoomId == roomId();
}

bool Room::isEqual(const Room &other) const
{
    return (mRoomId == other.roomId())
           && (mChannelType == other.channelType())
           && (mName == other.name())
           && (mAnnouncement == other.announcement())
           && (mRoomCreatorUserName == other.roomCreatorUserName())
           && (mRoomCreateUserId == other.roomCreatorUserId())
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
           && (mDescription == other.description())
           && (mUserMentions == other.userMentions())
           && (mNotificationOptions == other.notificationOptions())
           && (mUpdatedAt == other.updatedAt())
           && (mLastSeeAt == other.lastSeeAt())
           && (mBlocked == other.blocked())
           && (mRoles == other.roles())
           && (mIgnoredUsers == other.ignoredUsers());
}

QString Room::name() const
{
    return mName;
}

QDebug operator <<(QDebug d, const Room &t)
{
    d << "id :" << t.roomId();
    d << "type :" << t.channelType();
    d << "name :" << t.name();
    d << "mAnnouncement :" << t.announcement();
    d << "roomCreaterUserName :" << t.roomCreatorUserName();
    d << "roomCreaterUserID :" << t.roomCreatorUserId();
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
    d << "userMentions: " << t.userMentions();
    d << "notifications: " << t.notificationOptions();
    d << "UpdatedAt: " << t.updatedAt();
    d << "LastSeeAt: " << t.lastSeeAt();
    d << "blocked: " << t.blocked();
    d << "roles: " << t.roles();
    d << "ignoredUsers: " << t.ignoredUsers();
    return d;
}

bool Room::canBeModify() const
{
    if (mRocketChatAccount) {
        return mRoles.contains(QLatin1String("owner"));
    }
    return false;
}

NotificationOptions Room::notificationOptions() const
{
    return mNotificationOptions;
}

void Room::setNotificationOptions(const NotificationOptions &notificationOptions)
{
    mNotificationOptions = notificationOptions;
}

int Room::userMentions() const
{
    return mUserMentions;
}

void Room::setUserMentions(int userMentions)
{
    mUserMentions = userMentions;
}

void Room::updateSubscriptionRoom(const QJsonObject &json)
{
    parseSubscriptionRoom(json);
}

qint64 Room::updatedAt() const
{
    return mUpdatedAt;
}

void Room::setUpdatedAt(const qint64 &updatedAt)
{
    mUpdatedAt = updatedAt;
}

void Room::parseUpdateRoom(const QJsonObject &json)
{
    qCDebug(RUQOLA_LOG) << "void Room::parseUpdateRoom(const QJsonObject &json)"<<json;
    if (json.contains(QLatin1String("rid"))) {
        setRoomId(json.value(QLatin1String("rid")).toString());
    }
    setJitsiTimeout(Utils::parseDate(QStringLiteral("jitsiTimeout"), json));
    if (json.contains(QLatin1String("alert"))) {
        setAlert(json[QStringLiteral("alert")].toBool());
    }
    if (json.contains(QLatin1String("f"))) {
        setFavorite(json[QStringLiteral("f")].toBool());
    }

    if (json.contains(QLatin1String("unread"))) {
        setUnread(json[QStringLiteral("unread")].toInt());
    }
    if (json.contains(QLatin1String("userMentions"))) {
        setUserMentions(json[QStringLiteral("userMentions")].toInt());
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
    if (json.contains(QLatin1String("blocker"))) {
        setBlocker(json[QStringLiteral("blocker")].toBool());
    } else {
        setBlocker(false);
    }
    if (json.contains(QLatin1String("blocked"))) {
        setBlocked(json[QStringLiteral("blocked")].toBool());
    } else {
        setBlocked(false);
    }

    const QJsonArray ignoredArray = json.value(QLatin1String("ignored")).toArray();
    QStringList lstIgnored;
    lstIgnored.reserve(ignoredArray.count());
    for (int i = 0; i < ignoredArray.count(); ++i) {
        lstIgnored << ignoredArray.at(i).toString();
    }
    setIgnoredUsers(lstIgnored);

    //TODO muted ????

    const QJsonValue ownerValue = json.value(QLatin1String("u"));
    if (!ownerValue.isUndefined()) {
        const QJsonObject objOwner = ownerValue.toObject();
        setRoomCreatorUserId(objOwner.value(QLatin1String("_id")).toString());
        setRoomCreatorUserName(objOwner.value(QLatin1String("username")).toString());
    } else {
        //When room is initialized we are the owner. When we update room we have the real
        //owner and if it's empty => we need to clear it.
        setRoomCreatorUserId(QString());
        setRoomCreatorUserName(QString());
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
        //Add signal otherwise it's not necessary to check value
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
        //Add signal otherwise it's not necessary to check value
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
        Q_EMIT mutedUsersChanged();
    }
}

QString Room::roomCreatorUserId() const
{
    return mRoomCreateUserId;
}

void Room::setRoomCreatorUserId(const QString &userId)
{
    mRoomCreateUserId = userId;
}

QString Room::roomCreatorUserName() const
{
    return mRoomCreatorUserName;
}

void Room::setRoomCreatorUserName(const QString &userName)
{
    mRoomCreatorUserName = userName;
}

QString Room::roomId() const
{
    return mRoomId;
}

void Room::setRoomId(const QString &id)
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
        //TODO
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
    mChannelType = channelType;
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

void Room::parseInsertRoom(const QJsonObject &json)
{
    QString roomID = json.value(QLatin1String("_id")).toString();
    setRoomId(roomID);
    setName(json[QStringLiteral("name")].toString());
    setJitsiTimeout(Utils::parseDate(QStringLiteral("jitsiTimeout"), json));
    //topic/announcement/description is not part of update subscription
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
    if (json.contains(QLatin1String("userMentions"))) {
        setUserMentions(json[QStringLiteral("userMentions")].toInt());
    }
    if (json.contains(QLatin1String("announcement"))) {
        setAnnouncement(json[QStringLiteral("announcement")].toString());
    }
    if (json.contains(QLatin1String("description"))) {
        setDescription(json[QStringLiteral("description")].toString());
    }

    const QJsonArray ignoredArray = json.value(QLatin1String("ignored")).toArray();
    QStringList lstIgnored;
    lstIgnored.reserve(ignoredArray.count());
    for (int i = 0; i < ignoredArray.count(); ++i) {
        lstIgnored << ignoredArray.at(i).toString();
    }
    setIgnoredUsers(lstIgnored);


    setUpdatedAt(Utils::parseDate(QLatin1String("_updatedAt"), json));
    setUnread(json[QStringLiteral("unread")].toInt());
    setOpen(json[QStringLiteral("open")].toBool());
    setAlert(json[QStringLiteral("alert")].toBool());
    const QJsonValue blockerValue = json.value(QLatin1String("blocker"));
    if (!blockerValue.isUndefined()) {
        setBlocker(blockerValue.toBool());
    } else {
        setBlocker(false);
    }
    //Blocked ???
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

    const QJsonArray rolesArray = json.value(QLatin1String("roles")).toArray();
    QStringList lstRoles;
    lstRoles.reserve(rolesArray.count());
    for (int i = 0; i < rolesArray.count(); ++i) {
        lstRoles << rolesArray.at(i).toString();
    }
    setRoles(lstRoles);

    const QJsonValue ownerValue = json.value(QLatin1String("u"));
    if (!ownerValue.isUndefined()) {
        const QJsonObject objOwner = ownerValue.toObject();
        setRoomCreatorUserId(objOwner.value(QLatin1String("_id")).toString());
        setRoomCreatorUserName(objOwner.value(QLatin1String("username")).toString());
    } else {
        //When room is initialized we are the owner. When we update room we have the real
        //owner and if it's empty => we need to clear it.
        setRoomCreatorUserId(QString());
        setRoomCreatorUserName(QString());
    }
    //qDebug() << " *thus" << *this;
    mNotificationOptions.parseNotificationOptions(json);

    //TODO add muted
}

qint64 Room::lastSeeAt() const
{
    return mLastSeeAt;
}

void Room::setLastSeeAt(const qint64 &lastSeeAt)
{
    mLastSeeAt = lastSeeAt;
    //Add signal otherwise it's not necessary to check value
}

bool Room::blocked() const
{
    return mBlocked;
}

void Room::setBlocked(bool blocked)
{
    if (mBlocked != blocked) {
        mBlocked = blocked;
        Q_EMIT blockedChanged();
    }
}

QStringList Room::roles() const
{
    return mRoles;
}

void Room::setRoles(const QStringList &roles)
{
    if (mRoles != roles) {
        mRoles = roles;
        Q_EMIT rolesChanged();
    }
}

QStringList Room::ignoredUsers() const
{
    return mIgnoredUsers;
}

void Room::setIgnoredUsers(const QStringList &ignoredUsers)
{
    if (mIgnoredUsers != ignoredUsers) {
        mIgnoredUsers = ignoredUsers;
        Q_EMIT ignoredUsersChanged();
    }
}

void Room::parseSubscriptionRoom(const QJsonObject &json)
{
    QString roomID = json.value(QLatin1String("rid")).toString();
    if (roomID.isEmpty()) {
        roomID = json.value(QLatin1String("_id")).toString();
    }
    setRoomId(roomID);
    setName(json[QStringLiteral("name")].toString());
    setJitsiTimeout(Utils::parseDate(QStringLiteral("jitsiTimeout"), json));
    //topic/announcement/description is not part of update subscription
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

    setUpdatedAt(Utils::parseDate(QLatin1String("_updatedAt"), json));
    setUnread(json[QStringLiteral("unread")].toInt());
    setUserMentions(json[QStringLiteral("userMentions")].toInt());
    setOpen(json[QStringLiteral("open")].toBool());
    setAlert(json[QStringLiteral("alert")].toBool());
    const QJsonValue blockerValue = json.value(QLatin1String("blocker"));
    if (!blockerValue.isUndefined()) {
        setBlocker(blockerValue.toBool());
    } else {
        setBlocker(false);
    }
    //TODO blocked ?
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

    const QJsonArray ignoredArray = json.value(QLatin1String("ignored")).toArray();
    QStringList lstIgnored;
    lstIgnored.reserve(ignoredArray.count());
    for (int i = 0; i < ignoredArray.count(); ++i) {
        lstIgnored << ignoredArray.at(i).toString();
    }
    setIgnoredUsers(lstIgnored);


    const QJsonArray rolesArray = json.value(QLatin1String("roles")).toArray();
    QStringList lstRoles;
    lstRoles.reserve(rolesArray.count());
    for (int i = 0; i < rolesArray.count(); ++i) {
        lstRoles << rolesArray.at(i).toString();
    }
    setRoles(lstRoles);

//    const QJsonValue ownerValue = json.value(QLatin1String("u"));
//    if (!ownerValue.isUndefined()) {
//        const QJsonObject objOwner = ownerValue.toObject();
//        setRoomCreatorUserId(objOwner.value(QLatin1String("_id")).toString());
//        setRoomCreatorUserName(objOwner.value(QLatin1String("username")).toString());
//    } else {
//        //When room is initialized we are the owner. When we update room we have the real
//        //owner and if it's empty => we need to clear it.
//        setRoomCreatorUserId(QString());
//        setRoomCreatorUserName(QString());
//    }
    //qDebug() << " *thus" << *this;
    mNotificationOptions.parseNotificationOptions(json);

    //TODO add muted
}

Room *Room::fromJSon(const QJsonObject &o)
{
    //FIXME
    Room *r = new Room(nullptr);

    r->setRoomId(o[QStringLiteral("rid")].toString());
    r->setChannelType(o[QStringLiteral("t")].toString());
    r->setName(o[QStringLiteral("name")].toString());
    r->setRoomCreatorUserName(o[QStringLiteral("roomCreatorUserName")].toString());
    r->setRoomCreatorUserId(o[QStringLiteral("roomCreatorUserID")].toString());
    r->setTopic(o[QStringLiteral("topic")].toString());
    r->setJitsiTimeout(o[QStringLiteral("jitsiTimeout")].toDouble());
    r->setReadOnly(o[QStringLiteral("ro")].toBool());
    r->setUnread(o[QStringLiteral("unread")].toInt(0));
    r->setUserMentions(o[QStringLiteral("userMentions")].toInt(0));
    r->setAnnouncement(o[QStringLiteral("announcement")].toString());
    r->setSelected(o[QStringLiteral("selected")].toBool());
    r->setFavorite(o[QStringLiteral("favorite")].toBool());
    r->setAlert(o[QStringLiteral("alert")].toBool());
    r->setOpen(o[QStringLiteral("open")].toBool());
    r->setArchived(o[QStringLiteral("archived")].toBool());
    r->setDescription(o[QStringLiteral("description")].toString());
    r->setBlocker(o[QStringLiteral("blocker")].toBool());
    r->setBlocked(o[QStringLiteral("blocked")].toBool());
    r->setUpdatedAt(o[QStringLiteral("updatedAt")].toDouble());
    r->setLastSeeAt(o[QStringLiteral("lastSeeAt")].toDouble());
    const QJsonArray mutedArray = o.value(QLatin1String("mutedUsers")).toArray();
    QStringList lst;
    lst.reserve(mutedArray.count());
    for (int i = 0; i < mutedArray.count(); ++i) {
        lst <<mutedArray.at(i).toString();
    }
    r->setMutedUsers(lst);

    const QJsonArray ignoredArray = o.value(QLatin1String("ignored")).toArray();
    QStringList lstIgnored;
    lstIgnored.reserve(ignoredArray.count());
    for (int i = 0; i < ignoredArray.count(); ++i) {
        lstIgnored <<ignoredArray.at(i).toString();
    }
    r->setIgnoredUsers(lstIgnored);


    const QJsonArray rolesArray = o.value(QLatin1String("roles")).toArray();
    QStringList lstRoles;
    lst.reserve(rolesArray.count());
    for (int i = 0; i < rolesArray.count(); ++i) {
        lstRoles <<rolesArray.at(i).toString();
    }
    r->setRoles(lstRoles);

    const QJsonObject notificationsObj = o.value(QLatin1String("notifications")).toObject();
    const NotificationOptions notifications = NotificationOptions::fromJSon(notificationsObj);
    r->setNotificationOptions(notifications);

    return r;
}

QByteArray Room::serialize(Room *r, bool toBinary)
{
    QJsonDocument d;
    QJsonObject o;

    //todo add timestamp

    o[QStringLiteral("rid")] = r->roomId();
    o[QStringLiteral("t")] = r->channelType();
    o[QStringLiteral("name")] = r->name();
    o[QStringLiteral("roomCreatorUserName")] = r->roomCreatorUserName();
    o[QStringLiteral("roomCreatorUserID")] = r->roomCreatorUserId();
    if (!r->topic().isEmpty()) {
        o[QStringLiteral("topic")] = r->topic();
    }
    o[QStringLiteral("jitsiTimeout")] = r->jitsiTimeout();
    o[QStringLiteral("updatedAt")] = r->updatedAt();
    o[QStringLiteral("lastSeeAt")] = r->lastSeeAt();
    o[QStringLiteral("ro")] = r->readOnly();
    o[QStringLiteral("unread")] = r->unread();
    if (!r->announcement().isEmpty()) {
        o[QStringLiteral("announcement")] = r->announcement();
    }
    o[QStringLiteral("selected")] = r->selected();
    o[QStringLiteral("favorite")] = r->favorite();
    o[QStringLiteral("alert")] = r->alert();
    o[QStringLiteral("open")] = r->open();
    o[QStringLiteral("blocker")] = r->blocker();
    o[QStringLiteral("blocked")] = r->blocked();
    o[QStringLiteral("archived")] = r->archived();
    if (!r->description().isEmpty()) {
        o[QStringLiteral("description")] = r->description();
    }
    o[QStringLiteral("userMentions")] = r->userMentions();

    if (!r->mutedUsers().isEmpty()) {
        QJsonArray array;
        const int nbMuted = r->mutedUsers().count();
        for (int i = 0; i < nbMuted; ++i) {
            array.append(r->mutedUsers().at(i));
        }
        o[QStringLiteral("mutedUsers")] = array;
    }

    if (!r->ignoredUsers().isEmpty()) {
        QJsonArray array;
        const int nbIgnoredUsers = r->ignoredUsers().count();
        for (int i = 0; i < nbIgnoredUsers; ++i) {
            array.append(r->ignoredUsers().at(i));
        }
        o[QStringLiteral("ignored")] = array;
    }

    if (!r->roles().isEmpty()) {
        QJsonArray array;
        const int nbRoles = r->roles().count();
        for (int i = 0; i < nbRoles; ++i) {
            array.append(r->roles().at(i));
        }
        o[QStringLiteral("roles")] = array;
    }

    o[QStringLiteral("notifications")] = NotificationOptions::serialize(r->notificationOptions());

    d.setObject(o);
    if (toBinary) {
        return d.toBinaryData();
    }
    return d.toJson(QJsonDocument::Indented);
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

bool Room::userIsIgnored(const QString &userId)
{
    return mIgnoredUsers.contains(userId);
}
