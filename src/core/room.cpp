/*
 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "room.h"

#include "model/messagesmodel.h"
#include "model/usersforroommodel.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#include "ruqola_memory_management_debug.h"
#include "textconverter.h"

#include <KLocalizedString>
#include <QCborValue>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>

using namespace Qt::Literals::StringLiterals;
Room::Room(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mUsersModelForRoom(new UsersForRoomModel(this))
    , mRocketChatAccount(account)
{
    mUsersModelForRoom->setObjectName(u"usersforroommodel"_s);
    mMessageModel = new MessagesModel(QByteArray(), mRocketChatAccount, this, this);
}

Room::~Room() = default;

Room::RoomType Room::roomTypeFromString(const QString &type)
{
    if (type == "p"_L1) {
        return Room::RoomType::Private;
    } else if (type == "c"_L1) {
        return Room::RoomType::Channel;
    } else if (type == "d"_L1) {
        return Room::RoomType::Direct;
    } else {
        return Room::RoomType::Unknown;
    }
}

QString Room::roomFromRoomType(Room::RoomType type)
{
    switch (type) {
    case Room::RoomType::Private:
        return u"p"_s;
    case Room::RoomType::Channel:
        return u"c"_s;
    case Room::RoomType::Direct:
        return u"d"_s;
    case Room::RoomType::Unknown:
        qCDebug(RUQOLA_LOG) << "void Room::roomFromRoomType : unknown type";
        return {};
    }
    return {};
}

bool Room::operator==(const Room &other) const
{
    // qDebug() << " other.id"<<other.id << " id " << id;
    return other.mRoomId == roomId();
}

bool Room::isEqual(const Room &other) const
{
    return (mRoomId == other.roomId()) && (mChannelType == other.channelType()) && (mName == other.name()) && (mAnnouncement == other.announcement())
        && (mRoomCreatorUserName == other.roomOwnerUserName()) && (mRoomCreateUserId == other.roomCreatorUserId()) && (mTopic == other.topic())
        && (mutedUsers() == other.mutedUsers()) && (mJitsiTimeout == other.jitsiTimeout()) && (mUnread == other.unread())
        && (mDescription == other.description()) && (mUserMentions == other.userMentions()) && (mNotificationOptions == other.notificationOptions())
        && (mUpdatedAt == other.updatedAt()) && (mLastSeenAt == other.lastSeenAt()) && (mRoles == other.roles()) && (ignoredUsers() == other.ignoredUsers())
        && (parentRid() == other.parentRid()) && (mFName == other.fName()) && (autoTranslateLanguage() == other.autoTranslateLanguage())
        && (mDirectChannelUserId == other.directChannelUserId()) && (mDisplaySystemMessageType == other.displaySystemMessageTypes())
        && (mAvatarETag == other.avatarETag()) && (mUids == other.uids()) && (mUserNames == other.userNames()) && (highlightsWord() == other.highlightsWord())
        && (mRetentionInfo == other.retentionInfo()) && (teamInfo() == other.teamInfo()) && (mLastMessageAt == other.lastMessageAt())
        && (mGroupMentions == other.groupMentions()) && (mThreadUnread == other.threadUnread()) && (mRoomStates == other.roomStates())
        && e2EKey() == other.e2EKey() && e2eKeyId() == other.e2eKeyId();
}

QString Room::displayRoomName() const
{
    const QString displayName = mFName.isEmpty() ? mName : mFName;
    if (channelType() == RoomType::Direct) {
        return u'@' + displayName;
    } else {
        return u'#' + displayName;
    }
}

QString Room::name() const
{
    return mName;
}

QDebug operator<<(QDebug d, const Room &t)
{
    d.space() << "id :" << t.roomId();
    d.space() << "type :" << t.channelType();
    d.space() << "name :" << t.name();
    d.space() << "fname :" << t.fName();
    d.space() << "mAnnouncement :" << t.announcement();
    d.space() << "roomCreaterUserName :" << t.roomOwnerUserName();
    d.space() << "roomCreaterUserID :" << t.roomCreatorUserId();
    d.space() << "topic :" << t.topic();
    d.space() << "mutedUsers :" << t.mutedUsers();
    d.space() << "jitsiTimeout :" << t.jitsiTimeout();
    d.space() << "ro :" << t.readOnly();
    d.space() << "unread :" << t.unread();
    d.space() << "selected :" << t.selected();
    d.space() << "favorite :" << t.favorite();
    d.space() << "open :" << t.open();
    d.space() << "blocker: " << t.blocker();
    d.space() << "archived: " << t.archived();
    d.space() << "description: " << t.description();
    d.space() << "userMentions: " << t.userMentions();
    d.space() << "groupMentions: " << t.groupMentions();
    d.space() << "notifications: " << t.notificationOptions();
    d.space() << "UpdatedAt: " << t.updatedAt();
    d.space() << "LastSeenAt: " << t.lastSeenAt();
    d.space() << "LastMessageAt: " << t.lastMessageAt();
    d.space() << "blocked: " << t.blocked();
    d.space() << "roles: " << t.roles();
    d.space() << "ignoredUsers: " << t.ignoredUsers();
    d.space() << "encrypted room: " << t.encrypted();
    d.space() << "E2E keys: " << t.e2EKey();
    d.space() << "mE2eKeyId: " << t.e2eKeyId();
    d.space() << "mJoinCodeRequired: " << t.joinCodeRequired();
    d.space() << "broadcast: " << t.broadcast();
    d.space() << "ParentRid: " << t.parentRid();
    d.space() << "Fname: " << t.fName();
    d.space() << "autotranslate " << t.autoTranslate();
    d.space() << "autotranslateLanguage " << t.autoTranslateLanguage();
    d.space() << "directChannelUserId " << t.directChannelUserId();
    d.space() << "DisplaySystemMessageType " << t.displaySystemMessageTypes();
    d.space() << "AvatarEtag " << t.avatarETag();
    d.space() << "uids " << t.uids();
    d.space() << "usernames " << t.userNames();
    d.space() << "highlightsWord " << t.highlightsWord();
    d.space() << "RetentionInfo " << t.retentionInfo();
    d.space() << "TeamInfo " << t.teamInfo();
    d.space() << "Number Of messages in room " << t.numberMessages();
    d.space() << "threadUnread " << t.threadUnread();
    return d;
}

bool Room::canBeModify() const
{
    if (mRocketChatAccount) {
        return mRoles.contains("owner"_L1);
    }
    return false;
}

NotificationOptions Room::notificationOptions() const
{
    return mNotificationOptions;
}

void Room::setNotificationOptions(const NotificationOptions &notificationOptions)
{
    if (mNotificationOptions != notificationOptions) {
        mNotificationOptions = notificationOptions;
        Q_EMIT notificationOptionsChanged();
    }
}

int Room::userMentions() const
{
    return mUserMentions;
}

void Room::setUserMentions(int userMentions)
{
    mUserMentions = userMentions;
    // Send needAttention only if we have alert.
    if (mUserMentions > 0) {
        Q_EMIT needAttention();
    }
}

void Room::updateSubscriptionRoom(const QJsonObject &json)
{
    parseSubscriptionRoom(json);
}

qint64 Room::updatedAt() const
{
    return mUpdatedAt;
}

void Room::setUpdatedAt(qint64 updatedAt)
{
    mUpdatedAt = updatedAt;
}

void Room::parseUpdateRoom(const QJsonObject &json)
{
    qCDebug(RUQOLA_LOG) << "void Room::parseUpdateRoom(const QJsonObject &json)" << json;
    if (json.contains("rid"_L1)) {
        setRoomId(json.value("rid"_L1).toString().toLatin1());
    }
    setJitsiTimeout(Utils::parseDate(u"jitsiTimeout"_s, json));
    if (json.contains("alert"_L1)) {
        setAlert(json["alert"_L1].toBool());
    }
    if (json.contains("f"_L1)) {
        setFavorite(json["f"_L1].toBool());
    }

    if (json.contains("unread"_L1)) {
        setUnread(json["unread"_L1].toInt());
    }
    if (json.contains("userMentions"_L1)) {
        setUserMentions(json["userMentions"_L1].toInt());
    }
    if (json.contains("groupMentions"_L1)) {
        setGroupMentions(json["groupMentions"_L1].toInt());
    }
    if (json.contains("announcement"_L1)) {
        setAnnouncement(json["announcement"_L1].toString());
    }
    if (json.contains("description"_L1)) {
        setDescription(json["description"_L1].toString());
    }
    if (json.contains("open"_L1)) {
        setOpen(json["open"_L1].toBool());
    }
    if (json.contains("topic"_L1)) {
        setTopic(json["topic"_L1].toString());
    }
    if (json.contains("name"_L1)) {
        setName(json["name"_L1].toString());
    }
    if (json.contains("joinCodeRequired"_L1)) {
        setJoinCodeRequired(json["joinCodeRequired"_L1].toBool());
    } else {
        setJoinCodeRequired(false);
    }

    if (json.contains("fname"_L1)) {
        setFName(json["fname"_L1].toString());
    }
    if (json.contains("autoTranslateLanguage"_L1)) {
        setAutoTranslateLanguage(json["autoTranslateLanguage"_L1].toString());
    }
    if (json.contains("autoTranslate"_L1)) {
        setAutoTranslate(json["autoTranslate"_L1].toBool());
    }
    parseBlockerArchived(json);
    if (json.contains("blocked"_L1)) {
        setBlocked(json["blocked"_L1].toBool());
    } else {
        setBlocked(false);
    }

    if (json.contains("encrypted"_L1)) {
        setEncrypted(json["encrypted"_L1].toBool());
    } else {
        setEncrypted(false);
    }
    // TODO verify it. add autotest
    if (json.contains("broadcast"_L1)) {
        setBroadcast(json["broadcast"_L1].toBool());
    } else {
        setBroadcast(false);
    }
    setReadOnly(json["ro"_L1].toBool());
    const qint64 result = Utils::parseDate(u"ls"_s, json);
    if (result != -1) {
        setLastSeenAt(result);
    }
    const qint64 lm = Utils::parseDate(u"lm"_s, json);
    if (lm != -1) {
        setLastMessageAt(lm);
    }

    if (json.contains("msgs"_L1)) {
        mNumberMessages = json["msgs"_L1].toInt();
    }

    setHighlightsWord(extractStringList(json, "userHighlights"_L1));

    if (json.contains("ignored"_L1)) {
        setIgnoredUsers(extractStringList(json, "ignored"_L1));
    }

    // TODO E2EKey
    setE2eKeyId(json["e2eKeyId"_L1].toString());

    const QJsonValue ownerValue = json.value("u"_L1);
    if (!ownerValue.isUndefined()) {
        const QJsonObject objOwner = ownerValue.toObject();
        setRoomCreatorUserId(objOwner.value("_id"_L1).toString().toLatin1());
        setRoomCreatorUserName(objOwner.value("username"_L1).toString());
    } else {
        // When room is initialized we are the owner. When we update room we have the real
        // owner and if it's empty => we need to clear it.
        setRoomCreatorUserId(QByteArray());
        setRoomCreatorUserName(QString());
    }
    if (json.contains("prid"_L1)) {
        setParentRid(json["prid"_L1].toString().toLatin1());
    }
    if (json.contains("uids"_L1)) {
        const QJsonArray &uidsArray = json["uids"_L1].toArray();
        const auto &u0 = uidsArray[0].toString().toLatin1();
        const auto &u1 = uidsArray[1].toString().toLatin1();
        setDirectChannelUserId((u0 == mRocketChatAccount->userId()) ? u1 : u0);

        QStringList lstUids;
        lstUids.reserve(uidsArray.count());
        for (int i = 0; i < uidsArray.count(); ++i) {
            lstUids << uidsArray.at(i).toString();
        }
        setUids(lstUids);
    }

    const QJsonArray userNamesArray = json.value("usernames"_L1).toArray();
    QStringList lstUserNames;
    const int nbUserNamesArray = userNamesArray.count();
    lstUserNames.reserve(nbUserNamesArray);
    for (int i = 0; i < nbUserNamesArray; ++i) {
        lstUserNames << userNamesArray.at(i).toString();
    }
    setUserNames(lstUserNames);

    setMutedUsers(extractStringList(json, "muted"_L1));
    setAvatarETag(json.value("avatarETag"_L1).toString().toLatin1());
    parseDisplaySystemMessage(json);
    parseRetentionInfo(json);
    parseTeamInfo(json);
}

void Room::parseTeamInfo(const QJsonObject &json)
{
    TeamInfo info;
    info.parseTeamInfo(json);
    setTeamInfo(std::move(info));
}

QStringList Room::threadUnread() const
{
    return mThreadUnread;
}

void Room::setThreadUnread(const QStringList &newThreadUnread)
{
    mThreadUnread = newThreadUnread;
}

int Room::groupMentions() const
{
    return mGroupMentions;
}

void Room::setGroupMentions(int newGroupMentions)
{
    mGroupMentions = newGroupMentions;
}

qint64 Room::numberMessages() const
{
    return mNumberMessages;
}

void Room::setNumberMessages(qint64 newNumberMessages)
{
    mNumberMessages = newNumberMessages;
}

bool Room::selected() const
{
    return roomStateValue(Room::Selected);
}

void Room::setSelected(bool selected)
{
    assignRoomStateValue(Room::Selected, selected);
}

bool Room::hideBadgeForMention() const
{
    return mNotificationOptions.hideMentionStatus();
}

bool Room::hideUnreadStatus() const
{
    return mNotificationOptions.hideUnreadStatus();
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
    if (channelType() != RoomType::Channel) { // TODO verify it
        if (mUnread > 0) {
            Q_EMIT needAttention();
        }
    }
}

qint64 Room::jitsiTimeout() const
{
    return mJitsiTimeout;
}

void Room::setJitsiTimeout(qint64 jitsiTimeout)
{
    if (mJitsiTimeout != jitsiTimeout) {
        mJitsiTimeout = jitsiTimeout;
        Q_EMIT jitsiTimeoutChanged();
    }
}

QStringList Room::mutedUsers() const
{
    if (!mRoomExtra) {
        return {};
    }
    return mRoomExtra->mutedUsers();
}

void Room::setMutedUsers(const QStringList &users)
{
    if (mutedUsers() != users) {
        roomExtra()->setMutedUsers(users);
        Q_EMIT mutedUsersChanged();
    }
}

QByteArray Room::roomCreatorUserId() const
{
    return mRoomCreateUserId;
}

void Room::setRoomCreatorUserId(const QByteArray &userId)
{
    mRoomCreateUserId = userId;
}

QString Room::roomOwnerUserName() const
{
    return mRoomCreatorUserName;
}

void Room::setRoomCreatorUserName(const QString &userName)
{
    mRoomCreatorUserName = userName;
}

QByteArray Room::roomId() const
{
    return mRoomId;
}

void Room::setRoomId(const QByteArray &id)
{
    if (mRoomId != id) {
        mRoomId = id;
        mMessageModel->setRoomId(id);
    }
}

bool Room::alert() const
{
    return roomStateValue(Room::Alert);
}

void Room::setBlocker(bool block)
{
    if (roomStateValue(Room::Blocker) != block) {
        assignRoomStateValue(Room::Blocker, block);
        Q_EMIT blockerChanged();
    }
}

bool Room::blocker() const
{
    return roomStateValue(Room::Blocker);
}

void Room::setAlert(bool alert)
{
    if (roomStateValue(Room::Alert) != alert) {
        assignRoomStateValue(Room::Alert, alert);
        Q_EMIT alertChanged();
    }
}

bool Room::open() const
{
    return roomStateValue(Room::Open);
}

void Room::setOpen(bool open)
{
    if (roomStateValue(Room::Open) != open) {
        assignRoomStateValue(Room::Open, open);
        Q_EMIT openChanged(mRoomId);
    }
}

bool Room::readOnly() const
{
    return roomStateValue(Room::ReadOnly);
}

void Room::setReadOnly(bool readOnly)
{
    if (roomStateValue(Room::ReadOnly) != readOnly) {
        assignRoomStateValue(Room::ReadOnly, readOnly);
        Q_EMIT readOnlyChanged();
    }
}

QString Room::topic() const
{
    return mTopic;
}

QString Room::displayTopic() const
{
    if (mTopic.isEmpty()) {
        return {};
    }
    auto emojiManager = mRocketChatAccount ? mRocketChatAccount->emojiManager() : nullptr;
    auto messageCache = mRocketChatAccount ? mRocketChatAccount->messageCache() : nullptr;
    QByteArray needUpdateMessageId;
    const TextConverter::ConvertMessageTextSettings settings(mTopic, {}, {}, {}, emojiManager, messageCache, {}, {});
    int recursiveIndex = 0;
    return TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex);
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
    return roomStateValue(Room::Favorite);
}

void Room::setFavorite(bool favorite)
{
    if (roomStateValue(Room::Favorite) != favorite) {
        assignRoomStateValue(Room::Favorite, favorite);
        Q_EMIT favoriteChanged();
    }
}

Room::RoomType Room::channelType() const
{
    return mChannelType;
}

void Room::setChannelType(RoomType channelType)
{
    if (mChannelType != channelType) {
        mChannelType = channelType;
        Q_EMIT channelTypeChanged();
    }
}

QString Room::announcement() const
{
    return mAnnouncement;
}

QString Room::displayAnnouncement() const
{
    if (mAnnouncement.isEmpty()) {
        return {};
    }
    auto emojiManager = mRocketChatAccount ? mRocketChatAccount->emojiManager() : nullptr;
    auto messageCache = mRocketChatAccount ? mRocketChatAccount->messageCache() : nullptr;
    QByteArray needUpdateMessageId;
    const TextConverter::ConvertMessageTextSettings settings(mAnnouncement, {}, {}, {}, emojiManager, messageCache, {}, {});
    int recursiveIndex = 0;
    return TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex);
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
    const QByteArray roomID = json.value("_id"_L1).toString().toLatin1();
    // qDebug() << " json " << json;
    setRoomId(roomID);
    setName(json["name"_L1].toString());
    setFName(json["fname"_L1].toString());
    setAutoTranslateLanguage(json["autoTranslateLanguage"_L1].toString());
    setAutoTranslate(json["autoTranslate"_L1].toBool());
    setJitsiTimeout(Utils::parseDate(u"jitsiTimeout"_s, json));
    // topic/announcement/description is not part of update subscription
    const QString roomType = json.value("t"_L1).toString();
    setChannelType(Room::roomTypeFromString(roomType));
    const QJsonValue favoriteValue = json.value("f"_L1);
    if (!favoriteValue.isUndefined()) {
        setFavorite(favoriteValue.toBool());
    }
    setReadOnly(json["ro"_L1].toBool());
    if (json.contains("userMentions"_L1)) {
        setUserMentions(json["userMentions"_L1].toInt());
    }
    if (json.contains("groupMentions"_L1)) {
        setGroupMentions(json["groupMentions"_L1].toInt());
    }
    if (json.contains("announcement"_L1)) {
        setAnnouncement(json["announcement"_L1].toString());
    }
    if (json.contains("description"_L1)) {
        setDescription(json["description"_L1].toString());
    }
    if (json.contains("tunread"_L1)) {
        setThreadUnread(extractStringList(json, "tunread"_L1));
    }
    setUpdatedAt(Utils::parseDate(u"_updatedAt"_s, json));
    setLastSeenAt(Utils::parseDate(u"ls"_s, json));
    setLastMessageAt(Utils::parseDate(u"lm"_s, json));
    setUnread(json["unread"_L1].toInt());
    setOpen(json["open"_L1].toBool());
    setAlert(json["alert"_L1].toBool());

    parseBlockerArchived(json);

    // setE2eKeyId(json["e2eKeyId"_L1].toString());
    setE2EKey(json["E2EKey"_L1].toString());

    if (json.contains("encrypted"_L1)) {
        setEncrypted(json["encrypted"_L1].toBool());
    } else {
        setEncrypted(false);
    }

    parseCommonData(json);
    parseDisplaySystemMessage(json);

    const QJsonValue ownerValue = json.value("u"_L1);
    if (!ownerValue.isUndefined()) {
        const QJsonObject objOwner = ownerValue.toObject();
        setRoomCreatorUserId(objOwner.value("_id"_L1).toString().toLatin1());
        setRoomCreatorUserName(objOwner.value("username"_L1).toString());
    } else {
        // When room is initialized we are the owner. When we update room we have the real
        // owner and if it's empty => we need to clear it.
        setRoomCreatorUserId(QByteArray());
        setRoomCreatorUserName(QString());
    }
    // qDebug() << " *thus" << *this;
    mNotificationOptions.parseNotificationOptions(json);
    parseTeamInfo(json);
}

void Room::parseBlockerArchived(const QJsonObject &json)
{
    const QJsonValue blockerValue = json.value("blocker"_L1);
    if (blockerValue.isUndefined()) {
        setBlocker(false);
    } else {
        setBlocker(blockerValue.toBool());
    }
    // Blocked ???
    const QJsonValue archivedValue = json.value("archived"_L1);
    if (archivedValue.isUndefined()) {
        setArchived(false);
    } else {
        setArchived(archivedValue.toBool());
    }
}

void Room::parseSubscriptionRoom(const QJsonObject &json)
{
    QByteArray roomID = json.value("rid"_L1).toString().toLatin1();
    if (roomID.isEmpty()) {
        roomID = json.value("_id"_L1).toString().toLatin1();
    }
    setRoomId(roomID);
    setName(json["name"_L1].toString());
    setFName(json["fname"_L1].toString());
    setAutoTranslateLanguage(json["autoTranslateLanguage"_L1].toString());
    setAutoTranslate(json["autoTranslate"_L1].toBool());
    setJitsiTimeout(Utils::parseDate(u"jitsiTimeout"_s, json));
    // topic/announcement/description is not part of update subscription
    const QString roomType = json.value("t"_L1).toString();
    setChannelType(Room::roomTypeFromString(roomType));
    const QJsonValue favoriteValue = json.value("f"_L1);
    if (!favoriteValue.isUndefined()) {
        setFavorite(favoriteValue.toBool());
    }
    setE2EKey(json["E2EKey"_L1].toString());
    setReadOnly(json["ro"_L1].toBool());

    setUpdatedAt(Utils::parseDate(u"_updatedAt"_s, json));
    setLastSeenAt(Utils::parseDate(u"ls"_s, json));
    setUnread(json["unread"_L1].toInt());
    setUserMentions(json["userMentions"_L1].toInt());
    setGroupMentions(json["groupMentions"_L1].toInt());
    setOpen(json["open"_L1].toBool());
    setAlert(json["alert"_L1].toBool());
    parseBlockerArchived(json);
    // TODO e2ekey

    parseCommonData(json);
    parseDisplaySystemMessage(json);

    //    const QJsonValue ownerValue = json.value("u"_L1);
    //    if (!ownerValue.isUndefined()) {
    //        const QJsonObject objOwner = ownerValue.toObject();
    //        setRoomCreatorUserId(objOwner.value("_id"_L1).toString());
    //        setRoomCreatorUserName(objOwner.value("username"_L1).toString());
    //    } else {
    //        //When room is initialized we are the owner. When we update room we have the real
    //        //owner and if it's empty => we need to clear it.
    //        setRoomCreatorUserId(QString());
    //        setRoomCreatorUserName(QString());
    //    }
    // qDebug() << " *thus" << *this;
    mNotificationOptions.parseNotificationOptions(json);
    parseRetentionInfo(json);
    // parseTeamInfo(json);
}

qint64 Room::lastSeenAt() const
{
    return mLastSeenAt;
}

void Room::setLastSeenAt(qint64 lastSeenAt)
{
    if (mLastSeenAt != lastSeenAt) {
        mLastSeenAt = lastSeenAt;
        Q_EMIT lastSeenChanged();
    }
}

qint64 Room::lastMessageAt() const
{
    return mLastMessageAt;
}

void Room::setLastMessageAt(qint64 lastMessageAt)
{
    if (mLastMessageAt != lastMessageAt) {
        mLastMessageAt = lastMessageAt;
        Q_EMIT lastMessageAtChanged();
    }
}

bool Room::blocked() const
{
    return roomStateValue(Room::Blocked);
}

void Room::setBlocked(bool blocked)
{
    if (roomStateValue(Room::Blocked) != blocked) {
        assignRoomStateValue(Room::Blocked, blocked);
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
    if (!mRoomExtra) {
        return {};
    }
    return mRoomExtra->ignoredUsers();
}

void Room::setIgnoredUsers(const QStringList &users)
{
    if (ignoredUsers() != users) {
        roomExtra()->setIgnoredUsers(users);
        Q_EMIT ignoredUsersChanged();
    }
}

void Room::parseRetentionInfo(const QJsonObject &json)
{
    const QJsonValue retentionValue = json.value("retention"_L1);
    if (!retentionValue.isUndefined()) {
        mRetentionInfo.parseRetentionInfo(retentionValue.toObject());
    }
}

Room::TeamRoomInfo Room::teamRoomInfo() const
{
    if (mRocketChatAccount && teamInfo().isValid()) {
        if (!teamInfo().mainTeam() && !teamInfo().teamId().isEmpty()) {
            return mRocketChatAccount->roomFromTeamId(teamInfo().teamId());
        }
    }
    return {};
}

TeamInfo Room::teamInfo() const
{
    if (!mRoomExtra) {
        return {};
    }
    return mRoomExtra->teamInfo();
}

void Room::setTeamInfo(const TeamInfo &info)
{
    if (teamInfo() != info) {
        roomExtra()->setTeamInfo(info);
        Q_EMIT teamInfoChanged();
    }
}

void Room::parseDisplaySystemMessage(const QJsonObject &json)
{
    setDisplaySystemMessageTypes(extractStringList(json, "sysMes"_L1));
}

RetentionInfo Room::retentionInfo() const
{
    return mRetentionInfo;
}

void Room::setRetentionInfo(RetentionInfo retentionInfo)
{
    if (mRetentionInfo != retentionInfo) {
        mRetentionInfo = retentionInfo;
        Q_EMIT retentionInfoChanged();
    }
}

QStringList Room::highlightsWord() const
{
    if (!mRoomExtra) {
        return {};
    }
    return mRoomExtra->highlightsWord();
}

void Room::setHighlightsWord(const QStringList &words)
{
    if (highlightsWord() != words) {
        roomExtra()->setHighlightsWord(words);
        Q_EMIT highlightsWordChanged();
    }
}

QStringList Room::userNames() const
{
    return mUserNames;
}

void Room::setUserNames(const QStringList &userNames)
{
    if (mUserNames != userNames) {
        mUserNames = userNames;
        Q_EMIT userNamesChanged();
    }
}

QStringList Room::uids() const
{
    return mUids;
}

void Room::setUids(const QStringList &uids)
{
    if (mUids != uids) {
        mUids = uids;
        mCurrentAvatarInfo = {};
        Q_EMIT uidsChanged();
    }
}

Utils::AvatarInfo Room::avatarInfo() const
{
    if (mCurrentAvatarInfo.isValid()) {
        return mCurrentAvatarInfo;
    }
    // TODO direct channel or group channel
    Utils::AvatarInfo info;
    info.etag = QString::fromLatin1(mAvatarETag);
    // Group => uids >= 3
    const int uidsCount = mUids.count();
    if (uidsCount > 2) {
        QString identifier;
        for (const QString &username : mUserNames) {
            identifier.append(username);
        }
        identifier.prepend(QString::number(uidsCount));
        info.avatarType = Utils::AvatarType::User;
        info.identifier = identifier;
    } else if (uidsCount == 2) {
        info.avatarType = Utils::AvatarType::User;
        if (mRocketChatAccount) {
            QString otherUserName;
            for (const QString &userName : mUserNames) {
                if (userName != mRocketChatAccount->userName()) {
                    otherUserName = userName;
                }
            }
            info.identifier = otherUserName;
        }
    } else {
        info.avatarType = Utils::AvatarType::Room;
        info.identifier = QString::fromLatin1(mRoomId);
    }
    mCurrentAvatarInfo = info;
    return mCurrentAvatarInfo;
}

QByteArray Room::avatarETag() const
{
    return mAvatarETag;
}

void Room::setAvatarETag(const QByteArray &avatarETag)
{
    if (mAvatarETag != avatarETag) {
        mAvatarETag = avatarETag;
        mCurrentAvatarInfo = {};
        Q_EMIT avatarETagChanged();
    }
}

const ChannelCounterInfo *Room::channelCounterInfo() const
{
    if (mChannelCounterInfo) {
        return mChannelCounterInfo.data();
    }
    return nullptr;
}

void Room::setChannelCounterInfo(const ChannelCounterInfo &channelCounterInfo)
{
    if (channelCounterInfo.isValid()) {
        if (!mChannelCounterInfo) {
            mChannelCounterInfo = new ChannelCounterInfo(channelCounterInfo);
        } else {
            if (channelCounterInfo != *mChannelCounterInfo) {
                mChannelCounterInfo.reset(new ChannelCounterInfo(channelCounterInfo));
            }
        }
        Q_EMIT channelCounterInfoChanged();
    } else {
        if (mChannelCounterInfo) {
            mChannelCounterInfo.reset(nullptr);
            Q_EMIT channelCounterInfoChanged();
        }
    }
}

void Room::newMessageAdded()
{
    if (mChannelCounterInfo && mChannelCounterInfo->isValid()) {
        if (mChannelCounterInfo->unreadMessages() > 0) {
            const auto unreadMessageCount = mChannelCounterInfo->unreadMessages() + 1;
            mChannelCounterInfo->setUnreadMessages(unreadMessageCount);
            Q_EMIT channelCounterInfoChanged();
            // qDebug() << " mChannelCounterInfo " << mChannelCounterInfo;
        }
    }
}

void Room::parseCommonData(const QJsonObject &json)
{
    setMutedUsers(extractStringList(json, "muted"_L1));

    setIgnoredUsers(extractStringList(json, "ignored"_L1));
    setRoles(extractStringList(json, "roles"_L1));
    setThreadUnread(extractStringList(json, "tunread"_L1));
    setHighlightsWord(extractStringList(json, "userHighlights"_L1));
}

QStringList Room::displaySystemMessageTypes() const
{
    return mDisplaySystemMessageType;
}

void Room::setDisplaySystemMessageTypes(const QStringList &systemMessageType)
{
    if (mDisplaySystemMessageType != systemMessageType) {
        mDisplaySystemMessageType = systemMessageType;
        Q_EMIT displaySystemMessageTypesChanged();
    }
}

bool Room::autoTranslate() const
{
    return roomStateValue(Room::AutoTranslate);
}

void Room::setAutoTranslate(bool autoTranslate)
{
    if (roomStateValue(Room::AutoTranslate) != autoTranslate) {
        assignRoomStateValue(Room::AutoTranslate, autoTranslate);
        Q_EMIT autoTranslateChanged();
    }
}

QString Room::autoTranslateLanguage() const
{
    if (!mRoomExtra) {
        return {};
    }
    return mRoomExtra->autoTranslateLanguage();
}

void Room::setAutoTranslateLanguage(const QString &autotranslateLang)
{
    if (autoTranslateLanguage() != autotranslateLang) {
        roomExtra()->setAutoTranslateLanguage(autotranslateLang);
        Q_EMIT autoTranslateLanguageChanged();
    }
}

QString Room::displayFName() const
{
    if (mFName.isEmpty()) { // Fallback to name if fname is empty
        return mName;
    }
    return mFName;
}

QString Room::fName() const
{
    return mFName;
}

void Room::setFName(const QString &value)
{
    if (mFName != value) {
        mFName = value;
        Q_EMIT fnameChanged();
    }
}

bool Room::isDiscussionRoom() const
{
    if (!mRoomExtra) {
        return false;
    }
    return mRoomExtra->isDiscussionRoom();
}

QByteArray Room::parentRid() const
{
    if (!mRoomExtra) {
        return {};
    }
    return mRoomExtra->parentRid();
}

void Room::setParentRid(const QByteArray &rid)
{
    if (parentRid() != rid) {
        roomExtra()->setParentRid(rid);
        Q_EMIT parentRidChanged();
    }
}

bool Room::broadcast() const
{
    return roomStateValue(Room::BroadCast);
}

void Room::setBroadcast(bool broadcast)
{
    if (roomStateValue(Room::BroadCast) != broadcast) {
        assignRoomStateValue(Room::BroadCast, broadcast);
        Q_EMIT broadcastChanged();
    }
}

Roles Room::rolesForRooms() const
{
    return mRolesForRooms;
}

void Room::setRolesForRooms(const Roles &rolesForRooms)
{
    mRolesForRooms = rolesForRooms;
}

bool Room::hasPermission(const QString &permission) const
{
    if (mRocketChatAccount) {
        // qDebug() << " mRoles " << mRoles << " mRolesForRooms " << mRolesForRooms;
        const QStringList permissionRoles = mRocketChatAccount->permissions(permission);
        for (const QString &role : permissionRoles) {
            if (mRoles.contains(role)) {
                return true;
            }
        }
    }
    return false;
}

bool Room::allowToPinMessage() const
{
    return hasPermission(u"pin-message"_s);
}

QStringList Room::rolesForUserId(const QByteArray &userId)
{
    QStringList lstRoles;
    const Role r = mRolesForRooms.findRoleByUserId(userId);
    // qDebug() << " mRolesForRooms" << mRolesForRooms;
    if (r.isValid()) {
        if (r.isOwner()) {
            lstRoles.append(i18n("Owner"));
        }
        if (r.isLeader()) {
            lstRoles.append(i18n("Leader"));
        }
        if (r.isModerator()) {
            lstRoles.append(i18n("Moderator"));
        }
    }
    return lstRoles;
}

bool Room::wasInitialized() const
{
    return roomStateValue(Room::WasInitialized);
}

void Room::setWasInitialized(bool wasInitialized)
{
    assignRoomStateValue(Room::WasInitialized, wasInitialized);
}

bool Room::joinCodeRequired() const
{
    return roomStateValue(Room::JoinCodeRequired);
}

void Room::setJoinCodeRequired(bool joinCodeRequired)
{
    if (roomStateValue(Room::JoinCodeRequired) != joinCodeRequired) {
        assignRoomStateValue(Room::JoinCodeRequired, joinCodeRequired);
        Q_EMIT joinCodeRequiredChanged();
    }
}

QString Room::e2eKeyId() const
{
    if (mRoomEncryptionKey) {
        return mRoomEncryptionKey->e2eKeyId();
    }
    return {};
}

void Room::setE2eKeyId(const QString &e2eKeyId)
{
    if (mRoomEncryptionKey) {
        if (mRoomEncryptionKey->e2eKeyId() != e2eKeyId) {
            mRoomEncryptionKey->setE2eKeyId(e2eKeyId);
            Q_EMIT encryptionKeyIdChanged();
        }
    } else {
        mRoomEncryptionKey = new RoomEncryptionKey;
        mRoomEncryptionKey->setE2eKeyId(e2eKeyId);
        Q_EMIT encryptionKeyIdChanged();
    }
}

QString Room::e2EKey() const
{
    if (mRoomEncryptionKey) {
        return mRoomEncryptionKey->e2EKey();
    }
    return {};
}

void Room::setE2EKey(const QString &e2EKey)
{
    if (mRoomEncryptionKey) {
        if (mRoomEncryptionKey->e2EKey() != e2EKey) {
            mRoomEncryptionKey->setE2EKey(e2EKey);
            Q_EMIT encryptionKeyChanged();
        }
    } else {
        mRoomEncryptionKey = new RoomEncryptionKey;
        mRoomEncryptionKey->setE2EKey(e2EKey);
        Q_EMIT encryptionKeyIdChanged();
    }
}

bool Room::encrypted() const
{
    return roomStateValue(Room::Encrypted);
}

void Room::setEncrypted(bool encrypted)
{
    if (roomStateValue(Room::Encrypted) != encrypted) {
        assignRoomStateValue(Room::Encrypted, encrypted);
        Q_EMIT encryptedChanged();
    }
}

void Room::deserialize(Room *r, const QJsonObject &o)
{
    r->setRoomId(o["rid"_L1].toString().toLatin1());
    r->setChannelType(Room::roomTypeFromString(o["t"_L1].toString()));
    r->setName(o["name"_L1].toString());
    r->setFName(o["fname"_L1].toString());
    r->setAutoTranslateLanguage(o["autoTranslateLanguage"_L1].toString());
    r->setAutoTranslate(o["autoTranslate"_L1].toBool());
    r->setRoomCreatorUserName(o["roomCreatorUserName"_L1].toString());
    r->setRoomCreatorUserId(o["roomCreatorUserID"_L1].toString().toLatin1());
    r->setTopic(o["topic"_L1].toString());
    r->setJitsiTimeout(static_cast<qint64>(o["jitsiTimeout"_L1].toDouble()));
    r->setReadOnly(o["ro"_L1].toBool(false));
    r->setUnread(o["unread"_L1].toInt(0));
    r->setUserMentions(o["userMentions"_L1].toInt(0));
    r->setGroupMentions(o["groupMentions"_L1].toInt(0));
    r->setAnnouncement(o["announcement"_L1].toString());
    r->setSelected(o["selected"_L1].toBool(false));
    r->setFavorite(o["favorite"_L1].toBool(false));
    r->setAlert(o["alert"_L1].toBool(false));
    r->setOpen(o["open"_L1].toBool(false));
    r->setArchived(o["archived"_L1].toBool(false));
    r->setDescription(o["description"_L1].toString());
    r->setBlocker(o["blocker"_L1].toBool(false));
    r->setBlocked(o["blocked"_L1].toBool(false));
    r->setEncrypted(o["encrypted"_L1].toBool(false));
    r->setBroadcast(o["broadcast"_L1].toBool(false));
    r->setE2EKey(o["e2ekey"_L1].toString());
    r->setE2eKeyId(o["e2ekeyid"_L1].toString());
    r->setJoinCodeRequired(o["joinCodeRequired"_L1].toBool());
    r->setUpdatedAt(static_cast<qint64>(o["updatedAt"_L1].toDouble()));
    r->setLastSeenAt(static_cast<qint64>(o["lastSeenAt"_L1].toDouble()));
    r->setNumberMessages(static_cast<qint64>(o["msgs"_L1].toInt()));

    r->setMutedUsers(extractStringList(o, "muted"_L1));

    r->setDisplaySystemMessageTypes(extractStringList(o, "systemMessages"_L1));

    r->setIgnoredUsers(extractStringList(o, "ignored"_L1));

    r->setHighlightsWord(extractStringList(o, "userHighlights"_L1));

    r->setRoles(extractStringList(o, "roles"_L1));

    r->setThreadUnread(extractStringList(o, "tunread"_L1));

    const QJsonObject notificationsObj = o.value("notifications"_L1).toObject();
    const NotificationOptions notifications = NotificationOptions::deserialize(notificationsObj);
    r->setNotificationOptions(notifications);

    r->setDirectChannelUserId(o["directChannelUserId"_L1].toString().toLatin1());

    r->setAvatarETag(o["avatarETag"_L1].toString().toLatin1());

    r->setUids(extractStringList(o, "uids"_L1));

    const QJsonObject retentionObj = o.value("retention"_L1).toObject();
    const RetentionInfo retention = RetentionInfo::deserialize(retentionObj);
    r->setRetentionInfo(retention);
    const TeamInfo teaminfo = TeamInfo::deserialize(o);
    r->setTeamInfo(teaminfo);

    if (o.contains("prid"_L1)) {
        r->setParentRid(o["prid"_L1].toString().toLatin1());
    }

    r->setUserNames(extractStringList(o, "usernames"_L1));
}

QStringList Room::extractStringList(const QJsonObject &o, const QString &key)
{
    const QJsonArray array = o.value(key).toArray();
    QStringList lstElements;
    const auto nbArrayElement = array.count();
    lstElements.reserve(nbArrayElement);
    for (auto i = 0; i < nbArrayElement; ++i) {
        lstElements << array.at(i).toString();
    }
    return lstElements;
}

// For autotest only
std::unique_ptr<Room> Room::deserialize(const QJsonObject &o)
{
    auto r = std::make_unique<Room>(nullptr);
    deserialize(r.get(), o);
    return r;
}

QByteArray Room::serialize(Room *r, bool toBinary)
{
    QJsonDocument d;
    QJsonObject o;

    // todo add timestamp

    o["rid"_L1] = QString::fromLatin1(r->roomId());
    o["t"_L1] = Room::roomFromRoomType(r->channelType());
    o["name"_L1] = r->name();
    if (!r->fName().isEmpty()) {
        o["fname"_L1] = r->fName();
    }
    if (!r->roomOwnerUserName().isEmpty()) {
        o["roomCreatorUserName"_L1] = r->roomOwnerUserName();
    }
    if (!r->roomCreatorUserId().isEmpty()) {
        o["roomCreatorUserID"_L1] = QString::fromLatin1(r->roomCreatorUserId());
    }
    if (r->numberMessages() > 0) {
        o["msgs"_L1] = r->numberMessages();
    }
    if (!r->topic().isEmpty()) {
        o["topic"_L1] = r->topic();
    }
    if (!r->autoTranslateLanguage().isEmpty()) {
        o["autoTranslateLanguage"_L1] = r->autoTranslateLanguage();
    }
    if (r->autoTranslate()) {
        o["autoTranslate"_L1] = r->autoTranslate();
    }
    if (r->jitsiTimeout() != -1) {
        o["jitsiTimeout"_L1] = r->jitsiTimeout();
    }
    o["updatedAt"_L1] = r->updatedAt();
    o["lastSeenAt"_L1] = r->lastSeenAt();
    if (r->readOnly()) {
        o["ro"_L1] = true;
    }
    o["unread"_L1] = r->unread();
    if (!r->announcement().isEmpty()) {
        o["announcement"_L1] = r->announcement();
    }
    if (r->selected()) {
        o["selected"_L1] = true;
    }
    if (r->favorite()) {
        o["favorite"_L1] = true;
    }
    if (r->alert()) {
        o["alert"_L1] = true;
    }
    if (r->open()) {
        o["open"_L1] = true;
    }
    if (r->blocker()) {
        o["blocker"_L1] = true;
    }
    if (r->blocked()) {
        o["blocked"_L1] = true;
    }
    if (r->encrypted()) {
        o["encrypted"_L1] = true;
    }
    if (r->archived()) {
        o["archived"_L1] = true;
    }
    if (r->broadcast()) {
        o["broadcast"_L1] = true;
    }
    if (r->joinCodeRequired()) {
        o["joinCodeRequired"_L1] = true;
    }
    if (!r->e2EKey().isEmpty()) {
        o["e2ekey"_L1] = r->e2EKey();
    }
    if (!r->e2eKeyId().isEmpty()) {
        o["e2ekeyid"_L1] = r->e2eKeyId();
    }

    if (!r->description().isEmpty()) {
        o["description"_L1] = r->description();
    }
    o["userMentions"_L1] = r->userMentions();
    if (r->groupMentions() > 0) {
        o["groupMentions"_L1] = r->groupMentions();
    }

    serializeStringList(o, "muted"_L1, r->mutedUsers());

    serializeStringList(o, "ignored"_L1, r->ignoredUsers());
    serializeStringList(o, "tunread"_L1, r->threadUnread());

    serializeStringList(o, "roles"_L1, r->roles());

    o["notifications"_L1] = NotificationOptions::serialize(r->notificationOptions());

    if (!r->directChannelUserId().isEmpty()) {
        o["directChannelUserId"_L1] = QLatin1StringView(r->directChannelUserId());
    }

    serializeStringList(o, "systemMessages"_L1, r->displaySystemMessageTypes());

    serializeStringList(o, "userHighlights"_L1, r->highlightsWord());

    if (!r->avatarETag().isEmpty()) {
        o["avatarETag"_L1] = QLatin1StringView(r->avatarETag());
    }
    if (!r->uids().isEmpty()) {
        o["uids"_L1] = QJsonArray::fromStringList(r->uids());
    }

    if (r->retentionInfo().isNotDefault()) {
        o["retention"_L1] = RetentionInfo::serialize(r->retentionInfo());
    }
    if (r->teamInfo().isValid()) {
        TeamInfo::serialize(r->teamInfo(), o);
    }
    if (!r->parentRid().isEmpty()) {
        o["prid"_L1] = QLatin1StringView(r->parentRid());
    }

    serializeStringList(o, "usernames"_L1, r->userNames());

    if (toBinary) {
        return QCborValue::fromJsonValue(o).toCbor();
    }
    d.setObject(o);
    return d.toJson(QJsonDocument::Indented);
}

void Room::serializeStringList(QJsonObject &o, const QString &key, const QStringList &list)
{
    if (!list.isEmpty()) {
        QJsonArray array;
        const int nbListElement = list.count();
        for (int i = 0; i < nbListElement; ++i) {
            array.append(list.at(i));
        }
        o[key] = array;
    }
}

UsersForRoomModel *Room::usersModelForRoom() const
{
    return mUsersModelForRoom;
}

MessagesModel *Room::messageModel() const
{
    return mMessageModel;
}

bool Room::archived() const
{
    return roomStateValue(Room::Archived);
}

void Room::setArchived(bool archived)
{
    if (roomStateValue(Room::Archived) != archived) {
        assignRoomStateValue(Room::Archived, archived);
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

bool Room::encryptedEnabled() const
{
    return mRocketChatAccount ? mRocketChatAccount->ruqolaServerConfig()->encryptionEnabled() : false;
}

bool Room::userIsIgnored(const QByteArray &userId)
{
    const QStringList users = ignoredUsers();
    if (users.isEmpty()) {
        return false;
    }
    // TODO Convert mIgnoredUsers to QList<QByteArray>
    return users.contains(QString::fromLatin1(userId));
}

bool Room::userIsMuted(const QString &username)
{
    const QStringList users = mutedUsers();
    if (users.isEmpty()) {
        return false;
    }
    return users.contains(username);
}

bool Room::roomIsBlocked() const
{
    return ((readOnly() && !canChangeRoles()) || archived()) || blocker() || blocked();
}

QString Room::roomMessageInfo() const
{
    if ((readOnly() && !canChangeRoles()) || archived()) {
        return i18n("Channel is read only.");
    }
    if (blocker()) {
        return i18n("You have blocked this channel.");
    }
    if (blocked()) {
        return i18n("Channel was blocked.");
    }
    return {};
}

bool Room::canChangeRoles() const
{
    return mRoles.contains(u"owner"_s);
}

bool Room::userHasOwnerRole(const QByteArray &userId) const
{
    const Role r = mRolesForRooms.findRoleByUserId(userId);
    if (r.isValid()) {
        return r.isOwner();
    }
    return false;
}

bool Room::userHasLeaderRole(const QByteArray &userId) const
{
    const Role r = mRolesForRooms.findRoleByUserId(userId);
    if (r.isValid()) {
        return r.isLeader();
    }
    return false;
}

bool Room::userHasModeratorRole(const QByteArray &userId) const
{
    const Role r = mRolesForRooms.findRoleByUserId(userId);
    if (r.isValid()) {
        return r.isModerator();
    }
    return false;
}

void Room::updateRoles(const QJsonObject &obj)
{
    mRolesForRooms.updateRoles(obj);
}

QByteArray Room::directChannelUserId() const
{
    return mDirectChannelUserId;
}

void Room::setDirectChannelUserId(const QByteArray &uid)
{
    if (mDirectChannelUserId != uid) {
        mDirectChannelUserId = uid;
        Q_EMIT directChannelUserIdChanged();
    }
}

bool Room::roomStateValue(RoomState type) const
{
    return mRoomStates & type;
}

RoomExtra *Room::roomExtra()
{
    if (!mRoomExtra) {
        mRoomExtra = new RoomExtra;
    }
    return mRoomExtra;
}

qint64 Room::lastOpenedAt() const
{
    if (!mRoomExtra) {
        return -1;
    }
    return mRoomExtra->lastOpenedAt();
}

void Room::setLastOpenedAt(qint64 newLastOpenedAt)
{
    qCDebug(RUQOLA_MEMORY_MANAGEMENT_LOG) << "name " << name() << " newLastOpenedAt " << newLastOpenedAt;
    roomExtra()->setLastOpenedAt(newLastOpenedAt);
}

Room::RoomStates Room::roomStates() const
{
    return mRoomStates;
}

void Room::setRoomStates(RoomStates newRoomStates)
{
    mRoomStates = newRoomStates;
}

void Room::assignRoomStateValue(RoomState type, bool status)
{
    if (status) {
        mRoomStates |= type;
    } else {
        mRoomStates &= ~type;
    }
}

QIcon Room::icon() const
{
    if (teamInfo().mainTeam()) {
        return QIcon::fromTheme(u"group"_s);
    }

    // TODO add team icon support.
    switch (channelType()) {
    case Room::RoomType::Private:
        if (parentRid().isEmpty()) {
            return QIcon::fromTheme(u"lock"_s);
        } else {
            // TODO use a specific icon for discussion
        }
        break;
    case Room::RoomType::Channel:
        if (unread() > 0 || alert()) {
            return QIcon::fromTheme(u"irc-channel-active"_s);
        } else {
            return QIcon::fromTheme(u"irc-channel-inactive"_s);
        }
    case Room::RoomType::Direct: {
        if (mUids.count() > 2) {
            return QIcon::fromTheme(u"view-conversation-balloon-symbolic"_s);
        }
        const QString userStatusIconFileName = mRocketChatAccount ? mRocketChatAccount->userStatusIconFileName(name()) : QString();
        if (userStatusIconFileName.isEmpty()) {
            return QIcon::fromTheme(u"user-available"_s);
        } else {
            return QIcon::fromTheme(userStatusIconFileName);
        }
    }
    case Room::RoomType::Unknown:
        break;
    }
    return {};
}

void Room::clearHistory()
{
    if (mMessageModel) {
        mMessageModel->clearHistory();
    }
}

bool Room::canCleanHistory() const
{
    if (!mRoomExtra) {
        return false;
    }
    return mRoomExtra->canCleanHistory();
}

#include "moc_room.cpp"
