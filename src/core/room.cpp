/*
 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "room.h"
#include "model/messagesmodel.h"
#include "model/usersforroommodel.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#include "textconverter.h"

#include <KLocalizedString>

#include <QCborValue>
#include <QJsonArray>
#include <QJsonDocument>

Room::Room(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mUsersModelForRoom(new UsersForRoomModel(this))
    , mRocketChatAccount(account)
{
    mUsersModelForRoom->setObjectName(QStringLiteral("usersforroommodel"));
    mMessageModel = new MessagesModel(QByteArray(), mRocketChatAccount, this, this);
}

Room::~Room() = default;

Room::RoomType Room::roomTypeFromString(const QString &type)
{
    if (type == QLatin1StringView("p")) {
        return Room::RoomType::Private;
    } else if (type == QLatin1StringView("c")) {
        return Room::RoomType::Channel;
    } else if (type == QLatin1StringView("d")) {
        return Room::RoomType::Direct;
    } else {
        return Room::RoomType::Unknown;
    }
}

QString Room::roomFromRoomType(Room::RoomType type)
{
    switch (type) {
    case Room::RoomType::Private:
        return QStringLiteral("p");
    case Room::RoomType::Channel:
        return QStringLiteral("c");
    case Room::RoomType::Direct:
        return QStringLiteral("d");
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
        && (mMutedUsers == other.mutedUsers()) && (mJitsiTimeout == other.jitsiTimeout()) && (mReadOnly == other.readOnly()) && (mUnread == other.unread())
        && (mSelected == other.selected()) && (mFavorite == other.favorite()) && (mOpen == other.open()) && (mBlocker == other.blocker())
        && (mArchived == other.archived()) && (mDescription == other.description()) && (mUserMentions == other.userMentions())
        && (mNotificationOptions == other.notificationOptions()) && (mUpdatedAt == other.updatedAt()) && (mLastSeenAt == other.lastSeenAt())
        && (mBlocked == other.blocked()) && (mRoles == other.roles()) && (mIgnoredUsers == other.ignoredUsers()) && (mEncrypted == other.encrypted())
        && (mE2EKey == other.e2EKey()) && (mE2eKeyId == other.e2eKeyId()) && (mJoinCodeRequired == other.joinCodeRequired())
        && (mBroadcast == other.broadcast()) && (mParentRid == other.parentRid()) && (mFName == other.fName()) && (mAutoTranslate == other.autoTranslate())
        && (mAutotranslateLanguage == other.autoTranslateLanguage()) && (mDirectChannelUserId == other.directChannelUserId())
        && (mDisplaySystemMessageType == other.displaySystemMessageTypes()) && (mAvatarETag == other.avatarETag()) && (mUids == other.uids())
        && (mUserNames == other.userNames()) && (mHighlightsWord == other.highlightsWord()) && (mRetentionInfo == other.retentionInfo())
        && (mTeamInfo == other.teamInfo()) && (mLastMessageAt == other.lastMessageAt()) && (mGroupMentions == other.groupMentions())
        && (mThreadUnread == other.threadUnread());
}

QString Room::displayRoomName() const
{
    const QString displayName = mFName.isEmpty() ? mName : mFName;
    if (channelType() == RoomType::Direct) {
        return QLatin1Char('@') + displayName;
    } else {
        return QLatin1Char('#') + displayName;
    }
}

QString Room::name() const
{
    return mName;
}

QDebug operator<<(QDebug d, const Room &t)
{
    d << "id :" << t.roomId();
    d << "type :" << t.channelType();
    d << "name :" << t.name();
    d << "mAnnouncement :" << t.announcement();
    d << "roomCreaterUserName :" << t.roomOwnerUserName();
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
    d << "groupMentions: " << t.groupMentions();
    d << "notifications: " << t.notificationOptions();
    d << "UpdatedAt: " << t.updatedAt();
    d << "LastSeenAt: " << t.lastSeenAt();
    d << "LastMessageAt: " << t.lastMessageAt();
    d << "blocked: " << t.blocked();
    d << "roles: " << t.roles();
    d << "ignoredUsers: " << t.ignoredUsers();
    d << "encrypted room: " << t.encrypted();
    d << "E2E keys: " << t.e2EKey();
    d << "mE2eKeyId: " << t.e2eKeyId();
    d << "mJoinCodeRequired: " << t.joinCodeRequired();
    d << "broadcast: " << t.broadcast();
    d << "ParentRid: " << t.parentRid();
    d << "Fname: " << t.fName();
    d << "autotranslate " << t.autoTranslate();
    d << "autotranslateLanguage " << t.autoTranslateLanguage();
    d << "directChannelUserId " << t.directChannelUserId();
    d << "DisplaySystemMessageType " << t.displaySystemMessageTypes();
    d << "AvatarEtag " << t.avatarETag();
    d << "uids " << t.uids();
    d << "usernames " << t.userNames();
    d << "highlightsWord " << t.highlightsWord();
    d << "RetentionInfo " << t.retentionInfo();
    d << "TeamInfo " << t.teamInfo();
    d << "Number Of messages in room " << t.numberMessages();
    d << "threadUnread " << t.threadUnread();
    return d;
}

bool Room::canBeModify() const
{
    if (mRocketChatAccount) {
        return mRoles.contains(QLatin1StringView("owner"));
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
    if (json.contains(QLatin1StringView("rid"))) {
        setRoomId(json.value(QLatin1StringView("rid")).toString().toLatin1());
    }
    setJitsiTimeout(Utils::parseDate(QStringLiteral("jitsiTimeout"), json));
    if (json.contains(QLatin1StringView("alert"))) {
        setAlert(json[QLatin1StringView("alert")].toBool());
    }
    if (json.contains(QLatin1StringView("f"))) {
        setFavorite(json[QLatin1StringView("f")].toBool());
    }

    if (json.contains(QLatin1StringView("unread"))) {
        setUnread(json[QLatin1StringView("unread")].toInt());
    }
    if (json.contains(QLatin1StringView("userMentions"))) {
        setUserMentions(json[QLatin1StringView("userMentions")].toInt());
    }
    if (json.contains(QLatin1StringView("groupMentions"))) {
        setGroupMentions(json[QLatin1StringView("groupMentions")].toInt());
    }
    if (json.contains(QLatin1StringView("announcement"))) {
        setAnnouncement(json[QLatin1StringView("announcement")].toString());
    }
    if (json.contains(QLatin1StringView("description"))) {
        setDescription(json[QLatin1StringView("description")].toString());
    }
    if (json.contains(QLatin1StringView("open"))) {
        setOpen(json[QLatin1StringView("open")].toBool());
    }
    if (json.contains(QLatin1StringView("topic"))) {
        setTopic(json[QLatin1StringView("topic")].toString());
    }
    if (json.contains(QLatin1StringView("name"))) {
        setName(json[QLatin1StringView("name")].toString());
    }
    if (json.contains(QLatin1StringView("joinCodeRequired"))) {
        setJoinCodeRequired(json[QLatin1StringView("joinCodeRequired")].toBool());
    } else {
        setJoinCodeRequired(false);
    }

    if (json.contains(QLatin1StringView("fname"))) {
        setFName(json[QLatin1StringView("fname")].toString());
    }
    if (json.contains(QLatin1StringView("autoTranslateLanguage"))) {
        setAutoTranslateLanguage(json[QLatin1StringView("autoTranslateLanguage")].toString());
    }
    if (json.contains(QLatin1StringView("autoTranslate"))) {
        setAutoTranslate(json[QLatin1StringView("autoTranslate")].toBool());
    }
    if (json.contains(QLatin1StringView("archived"))) {
        setArchived(json[QLatin1StringView("archived")].toBool());
    } else {
        setArchived(false);
    }
    if (json.contains(QLatin1StringView("blocker"))) {
        setBlocker(json[QLatin1StringView("blocker")].toBool());
    } else {
        setBlocker(false);
    }
    if (json.contains(QLatin1StringView("blocked"))) {
        setBlocked(json[QLatin1StringView("blocked")].toBool());
    } else {
        setBlocked(false);
    }

    if (json.contains(QLatin1StringView("encrypted"))) {
        setEncrypted(json[QLatin1StringView("encrypted")].toBool());
    } else {
        setEncrypted(false);
    }
    // TODO verify it. add autotest
    if (json.contains(QLatin1StringView("broadcast"))) {
        setBroadcast(json[QLatin1StringView("broadcast")].toBool());
    } else {
        setBroadcast(false);
    }
    setReadOnly(json[QLatin1StringView("ro")].toBool());
    const qint64 result = Utils::parseDate(QStringLiteral("ls"), json);
    if (result != -1) {
        setLastSeenAt(result);
    }
    const qint64 lm = Utils::parseDate(QStringLiteral("lm"), json);
    if (lm != -1) {
        setLastMessageAt(lm);
    }

    if (json.contains(QLatin1StringView("msgs"))) {
        mNumberMessages = json[QLatin1StringView("msgs")].toInt();
    }

    setHighlightsWord(extractStringList(json, QLatin1StringView("userHighlights")));

    if (json.contains(QLatin1StringView("ignored"))) {
        setIgnoredUsers(extractStringList(json, QLatin1StringView("ignored")));
    }

    // TODO muted ????
    // TODO E2EKey
    setE2eKeyId(json[QLatin1StringView("e2eKeyId")].toString());

    const QJsonValue ownerValue = json.value(QLatin1StringView("u"));
    if (!ownerValue.isUndefined()) {
        const QJsonObject objOwner = ownerValue.toObject();
        setRoomCreatorUserId(objOwner.value(QLatin1StringView("_id")).toString().toLatin1());
        setRoomCreatorUserName(objOwner.value(QLatin1StringView("username")).toString());
    } else {
        // When room is initialized we are the owner. When we update room we have the real
        // owner and if it's empty => we need to clear it.
        setRoomCreatorUserId(QByteArray());
        setRoomCreatorUserName(QString());
    }
    if (json.contains(QLatin1StringView("prid"))) {
        setParentRid(json[QLatin1StringView("prid")].toString().toLatin1());
    }
    if (json.contains(QLatin1StringView("uids"))) {
        const QJsonArray &uidsArray = json[QLatin1StringView("uids")].toArray();
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

    const QJsonArray userNamesArray = json.value(QLatin1StringView("usernames")).toArray();
    QStringList lstUserNames;
    const int nbUserNamesArray = userNamesArray.count();
    lstUserNames.reserve(nbUserNamesArray);
    for (int i = 0; i < nbUserNamesArray; ++i) {
        lstUserNames << userNamesArray.at(i).toString();
    }
    setUserNames(lstUserNames);

    setAvatarETag(json.value(QLatin1StringView("avatarETag")).toString());
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
    return mSelected;
}

void Room::setSelected(bool selected)
{
    if (mSelected != selected) {
        mSelected = selected;
        // Add signal otherwise it's not necessary to check value
    }
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
    return mMutedUsers;
}

void Room::setMutedUsers(const QStringList &mutedUsers)
{
    if (mMutedUsers != mutedUsers) {
        mMutedUsers = mutedUsers;
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
        Q_EMIT openChanged();
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
    return mFavorite;
}

void Room::setFavorite(bool favorite)
{
    if (mFavorite != favorite) {
        mFavorite = favorite;
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
    const QByteArray roomID = json.value(QLatin1StringView("_id")).toString().toLatin1();
    // qDebug() << " json " << json;
    setRoomId(roomID);
    setName(json[QLatin1StringView("name")].toString());
    setFName(json[QLatin1StringView("fname")].toString());
    setAutoTranslateLanguage(json[QLatin1StringView("autoTranslateLanguage")].toString());
    setAutoTranslate(json[QLatin1StringView("autoTranslate")].toBool());
    setJitsiTimeout(Utils::parseDate(QStringLiteral("jitsiTimeout"), json));
    // topic/announcement/description is not part of update subscription
    const QString roomType = json.value(QLatin1StringView("t")).toString();
    setChannelType(Room::roomTypeFromString(roomType));
    const QJsonValue favoriteValue = json.value(QLatin1StringView("f"));
    if (!favoriteValue.isUndefined()) {
        setFavorite(favoriteValue.toBool());
    }
    setReadOnly(json[QLatin1StringView("ro")].toBool());
    if (json.contains(QLatin1StringView("userMentions"))) {
        setUserMentions(json[QLatin1StringView("userMentions")].toInt());
    }
    if (json.contains(QLatin1StringView("groupMentions"))) {
        setGroupMentions(json[QLatin1StringView("groupMentions")].toInt());
    }
    if (json.contains(QLatin1StringView("announcement"))) {
        setAnnouncement(json[QLatin1StringView("announcement")].toString());
    }
    if (json.contains(QLatin1StringView("description"))) {
        setDescription(json[QLatin1StringView("description")].toString());
    }
    if (json.contains(QLatin1StringView("tunread"))) {
        setThreadUnread(extractStringList(json, QLatin1StringView("tunread")));
    }
    setUpdatedAt(Utils::parseDate(QStringLiteral("_updatedAt"), json));
    setLastSeenAt(Utils::parseDate(QStringLiteral("ls"), json));
    setLastMessageAt(Utils::parseDate(QStringLiteral("lm"), json));
    setUnread(json[QLatin1StringView("unread")].toInt());
    setOpen(json[QLatin1StringView("open")].toBool());
    setAlert(json[QLatin1StringView("alert")].toBool());
    const QJsonValue blockerValue = json.value(QLatin1StringView("blocker"));
    if (!blockerValue.isUndefined()) {
        setBlocker(blockerValue.toBool());
    } else {
        setBlocker(false);
    }

    // setE2eKeyId(json[QLatin1StringView("e2eKeyId")].toString());
    setE2EKey(json[QLatin1StringView("E2EKey")].toString());

    if (json.contains(QLatin1StringView("encrypted"))) {
        setEncrypted(json[QLatin1StringView("encrypted")].toBool());
    } else {
        setEncrypted(false);
    }

    // Blocked ???
    const QJsonValue archivedValue = json.value(QLatin1StringView("archived"));
    if (!archivedValue.isUndefined()) {
        setArchived(archivedValue.toBool());
    } else {
        setArchived(false);
    }

    parseCommonData(json);
    parseDisplaySystemMessage(json);

    const QJsonValue ownerValue = json.value(QLatin1StringView("u"));
    if (!ownerValue.isUndefined()) {
        const QJsonObject objOwner = ownerValue.toObject();
        setRoomCreatorUserId(objOwner.value(QLatin1StringView("_id")).toString().toLatin1());
        setRoomCreatorUserName(objOwner.value(QLatin1StringView("username")).toString());
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
    QByteArray roomID = json.value(QLatin1StringView("rid")).toString().toLatin1();
    if (roomID.isEmpty()) {
        roomID = json.value(QLatin1StringView("_id")).toString().toLatin1();
    }
    setRoomId(roomID);
    setName(json[QLatin1StringView("name")].toString());
    setFName(json[QLatin1StringView("fname")].toString());
    setAutoTranslateLanguage(json[QLatin1StringView("autoTranslateLanguage")].toString());
    setAutoTranslate(json[QLatin1StringView("autoTranslate")].toBool());
    setJitsiTimeout(Utils::parseDate(QStringLiteral("jitsiTimeout"), json));
    // topic/announcement/description is not part of update subscription
    const QString roomType = json.value(QLatin1StringView("t")).toString();
    setChannelType(Room::roomTypeFromString(roomType));
    const QJsonValue favoriteValue = json.value(QLatin1StringView("f"));
    if (!favoriteValue.isUndefined()) {
        setFavorite(favoriteValue.toBool());
    }
    setE2EKey(json[QLatin1StringView("E2EKey")].toString());
    setReadOnly(json[QLatin1StringView("ro")].toBool());

    setUpdatedAt(Utils::parseDate(QStringLiteral("_updatedAt"), json));
    setLastSeenAt(Utils::parseDate(QStringLiteral("ls"), json));
    setUnread(json[QLatin1StringView("unread")].toInt());
    setUserMentions(json[QLatin1StringView("userMentions")].toInt());
    setGroupMentions(json[QLatin1StringView("groupMentions")].toInt());
    setOpen(json[QLatin1StringView("open")].toBool());
    setAlert(json[QLatin1StringView("alert")].toBool());
    const QJsonValue blockerValue = json.value(QLatin1StringView("blocker"));
    if (!blockerValue.isUndefined()) {
        setBlocker(blockerValue.toBool());
    } else {
        setBlocker(false);
    }
    // TODO e2ekey
    // TODO blocked ?
    const QJsonValue archivedValue = json.value(QLatin1StringView("archived"));
    if (!archivedValue.isUndefined()) {
        setArchived(archivedValue.toBool());
    } else {
        setArchived(false);
    }

    parseCommonData(json);
    parseDisplaySystemMessage(json);

    //    const QJsonValue ownerValue = json.value(QLatin1StringView("u"));
    //    if (!ownerValue.isUndefined()) {
    //        const QJsonObject objOwner = ownerValue.toObject();
    //        setRoomCreatorUserId(objOwner.value(QLatin1StringView("_id")).toString());
    //        setRoomCreatorUserName(objOwner.value(QLatin1StringView("username")).toString());
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
    // TODO add muted
}

void Room::parseRetentionInfo(const QJsonObject &json)
{
    const QJsonValue retentionValue = json.value(QLatin1StringView("retention"));
    if (!retentionValue.isUndefined()) {
        mRetentionInfo.parseRetentionInfo(retentionValue.toObject());
    }
}

Room::TeamRoomInfo Room::teamRoomInfo() const
{
    if (mRocketChatAccount) {
        if (!mTeamInfo.mainTeam() && !mTeamInfo.teamId().isEmpty()) {
            return mRocketChatAccount->roomFromTeamId(mTeamInfo.teamId());
        }
    }
    return {};
}

TeamInfo Room::teamInfo() const
{
    return mTeamInfo;
}

void Room::setTeamInfo(const TeamInfo &teamInfo)
{
    if (mTeamInfo != teamInfo) {
        mTeamInfo = teamInfo;
        Q_EMIT teamInfoChanged();
    }
}

void Room::parseDisplaySystemMessage(const QJsonObject &json)
{
    setDisplaySystemMessageTypes(extractStringList(json, QLatin1StringView("sysMes")));
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
    return mHighlightsWord;
}

void Room::setHighlightsWord(const QStringList &highlightsWord)
{
    if (mHighlightsWord != highlightsWord) {
        mHighlightsWord = highlightsWord;
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
    info.etag = mAvatarETag;
    // Group => uids >= 3
    if (mUids.count() > 2) {
        QString identifier;
        for (const QString &username : mUserNames) {
            identifier.append(username);
        }
        identifier.prepend(QString::number(mUids.count()));
        info.avatarType = Utils::AvatarType::User;
        info.identifier = identifier;
    } else if (mUids.count() == 2) {
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

QString Room::avatarETag() const
{
    return mAvatarETag;
}

void Room::setAvatarETag(const QString &avatarETag)
{
    if (mAvatarETag != avatarETag) {
        mAvatarETag = avatarETag;
        mCurrentAvatarInfo = {};
        Q_EMIT avatarETagChanged();
    }
}

ChannelCounterInfo Room::channelCounterInfo() const
{
    return mChannelCounterInfo;
}

void Room::setChannelCounterInfo(const ChannelCounterInfo &channelCounterInfo)
{
    if (mChannelCounterInfo != channelCounterInfo) {
        mChannelCounterInfo = channelCounterInfo;
        Q_EMIT channelCounterInfoChanged();
    }
}

void Room::newMessageAdded()
{
    if (mChannelCounterInfo.isValid()) {
        if (mChannelCounterInfo.unreadMessages() > 0) {
            const auto unreadMessageCount = mChannelCounterInfo.unreadMessages() + 1;
            mChannelCounterInfo.setUnreadMessages(unreadMessageCount);
            Q_EMIT channelCounterInfoChanged();
            // qDebug() << " mChannelCounterInfo " << mChannelCounterInfo;
        }
    }
}

void Room::parseCommonData(const QJsonObject &json)
{
    setMutedUsers(extractStringList(json, QLatin1StringView("muted")));

    setIgnoredUsers(extractStringList(json, QLatin1StringView("ignored")));
    setRoles(extractStringList(json, QLatin1StringView("roles")));
    setThreadUnread(extractStringList(json, QLatin1StringView("tunread")));

    // FIXME.
    setHighlightsWord(extractStringList(json, QLatin1StringView("userHighlights")));
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
    return mAutoTranslate;
}

void Room::setAutoTranslate(bool autoTranslate)
{
    if (mAutoTranslate != autoTranslate) {
        mAutoTranslate = autoTranslate;
        Q_EMIT autoTranslateChanged();
    }
}

QString Room::autoTranslateLanguage() const
{
    return mAutotranslateLanguage;
}

void Room::setAutoTranslateLanguage(const QString &autotranslateLanguage)
{
    if (mAutotranslateLanguage != autotranslateLanguage) {
        mAutotranslateLanguage = autotranslateLanguage;
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
    return !mParentRid.isEmpty();
}

QByteArray Room::parentRid() const
{
    return mParentRid;
}

void Room::setParentRid(const QByteArray &parentRid)
{
    if (mParentRid != parentRid) {
        mParentRid = parentRid;
        Q_EMIT parentRidChanged();
    }
}

bool Room::broadcast() const
{
    return mBroadcast;
}

void Room::setBroadcast(bool broadcast)
{
    if (mBroadcast != broadcast) {
        mBroadcast = broadcast;
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
    return hasPermission(QStringLiteral("pin-message"));
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
    return mWasInitialized;
}

void Room::setWasInitialized(bool wasInitialized)
{
    mWasInitialized = wasInitialized;
}

bool Room::joinCodeRequired() const
{
    return mJoinCodeRequired;
}

void Room::setJoinCodeRequired(bool joinCodeRequired)
{
    if (mJoinCodeRequired != joinCodeRequired) {
        mJoinCodeRequired = joinCodeRequired;
        Q_EMIT joinCodeRequiredChanged();
    }
}

QString Room::e2eKeyId() const
{
    return mE2eKeyId;
}

void Room::setE2eKeyId(const QString &e2eKeyId)
{
    if (mE2eKeyId != e2eKeyId) {
        mE2eKeyId = e2eKeyId;
        Q_EMIT encryptionKeyIdChanged();
    }
}

QString Room::e2EKey() const
{
    return mE2EKey;
}

void Room::setE2EKey(const QString &e2EKey)
{
    if (mE2EKey != e2EKey) {
        mE2EKey = e2EKey;
        Q_EMIT encryptionKeyChanged();
    }
}

bool Room::encrypted() const
{
    return mEncrypted;
}

void Room::setEncrypted(bool encrypted)
{
    if (mEncrypted != encrypted) {
        mEncrypted = encrypted;
        Q_EMIT encryptedChanged();
    }
}

void Room::deserialize(Room *r, const QJsonObject &o)
{
    r->setRoomId(o[QLatin1StringView("rid")].toString().toLatin1());
    r->setChannelType(Room::roomTypeFromString(o[QLatin1StringView("t")].toString()));
    r->setName(o[QLatin1StringView("name")].toString());
    r->setFName(o[QLatin1StringView("fname")].toString());
    r->setAutoTranslateLanguage(o[QLatin1StringView("autoTranslateLanguage")].toString());
    r->setAutoTranslate(o[QLatin1StringView("autoTranslate")].toBool());
    r->setRoomCreatorUserName(o[QLatin1StringView("roomCreatorUserName")].toString());
    r->setRoomCreatorUserId(o[QLatin1StringView("roomCreatorUserID")].toString().toLatin1());
    r->setTopic(o[QLatin1StringView("topic")].toString());
    r->setJitsiTimeout(static_cast<qint64>(o[QLatin1StringView("jitsiTimeout")].toDouble()));
    r->setReadOnly(o[QLatin1StringView("ro")].toBool());
    r->setUnread(o[QLatin1StringView("unread")].toInt(0));
    r->setUserMentions(o[QLatin1StringView("userMentions")].toInt(0));
    r->setGroupMentions(o[QLatin1StringView("groupMentions")].toInt(0));
    r->setAnnouncement(o[QLatin1StringView("announcement")].toString());
    r->setSelected(o[QLatin1StringView("selected")].toBool());
    r->setFavorite(o[QLatin1StringView("favorite")].toBool());
    r->setAlert(o[QLatin1StringView("alert")].toBool());
    r->setOpen(o[QLatin1StringView("open")].toBool());
    r->setArchived(o[QLatin1StringView("archived")].toBool());
    r->setDescription(o[QLatin1StringView("description")].toString());
    r->setBlocker(o[QLatin1StringView("blocker")].toBool());
    r->setBlocked(o[QLatin1StringView("blocked")].toBool());
    r->setEncrypted(o[QLatin1StringView("encrypted")].toBool());
    r->setBroadcast(o[QLatin1StringView("broadcast")].toBool());
    r->setE2EKey(o[QLatin1StringView("e2ekey")].toString());
    r->setE2eKeyId(o[QLatin1StringView("e2ekeyid")].toString());
    r->setJoinCodeRequired(o[QLatin1StringView("joinCodeRequired")].toBool());
    r->setUpdatedAt(static_cast<qint64>(o[QLatin1StringView("updatedAt")].toDouble()));
    r->setLastSeenAt(static_cast<qint64>(o[QLatin1StringView("lastSeenAt")].toDouble()));
    r->setNumberMessages(static_cast<qint64>(o[QLatin1StringView("msgs")].toInt()));

    r->setMutedUsers(extractStringList(o, QLatin1StringView("mutedUsers")));

    r->setDisplaySystemMessageTypes(extractStringList(o, QLatin1StringView("systemMessages")));

    r->setIgnoredUsers(extractStringList(o, QLatin1StringView("ignored")));

    r->setHighlightsWord(extractStringList(o, QLatin1StringView("userHighlights")));

    r->setRoles(extractStringList(o, QLatin1StringView("roles")));

    r->setThreadUnread(extractStringList(o, QLatin1StringView("tunread")));

    const QJsonObject notificationsObj = o.value(QLatin1StringView("notifications")).toObject();
    const NotificationOptions notifications = NotificationOptions::deserialize(notificationsObj);
    r->setNotificationOptions(notifications);

    r->setDirectChannelUserId(o[QLatin1StringView("directChannelUserId")].toString().toLatin1());

    r->setAvatarETag(o[QLatin1StringView("avatarETag")].toString());

    r->setUids(extractStringList(o, QLatin1StringView("uids")));

    const QJsonObject retentionObj = o.value(QLatin1StringView("retention")).toObject();
    const RetentionInfo retention = RetentionInfo::deserialize(retentionObj);
    r->setRetentionInfo(retention);
    const TeamInfo teaminfo = TeamInfo::deserialize(o);
    r->setTeamInfo(teaminfo);

    r->setParentRid(o[QLatin1StringView("prid")].toString().toLatin1());

    r->setUserNames(extractStringList(o, QLatin1StringView("usernames")));
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

    o[QLatin1StringView("rid")] = QString::fromLatin1(r->roomId());
    o[QLatin1StringView("t")] = Room::roomFromRoomType(r->channelType());
    o[QLatin1StringView("name")] = r->name();
    o[QLatin1StringView("fname")] = r->fName();
    o[QLatin1StringView("roomCreatorUserName")] = r->roomOwnerUserName();
    o[QLatin1StringView("roomCreatorUserID")] = QString::fromLatin1(r->roomCreatorUserId());
    if (r->numberMessages() > 0) {
        o[QLatin1StringView("msgs")] = r->numberMessages();
    }
    if (!r->topic().isEmpty()) {
        o[QLatin1StringView("topic")] = r->topic();
    }
    if (!r->autoTranslateLanguage().isEmpty()) {
        o[QLatin1StringView("autoTranslateLanguage")] = r->autoTranslateLanguage();
    }
    if (r->autoTranslate()) {
        o[QLatin1StringView("autoTranslate")] = r->autoTranslate();
    }
    o[QLatin1StringView("jitsiTimeout")] = r->jitsiTimeout();
    o[QLatin1StringView("updatedAt")] = r->updatedAt();
    o[QLatin1StringView("lastSeenAt")] = r->lastSeenAt();
    o[QLatin1StringView("ro")] = r->readOnly();
    o[QLatin1StringView("unread")] = r->unread();
    if (!r->announcement().isEmpty()) {
        o[QLatin1StringView("announcement")] = r->announcement();
    }
    o[QLatin1StringView("selected")] = r->selected();
    o[QLatin1StringView("favorite")] = r->favorite();
    o[QLatin1StringView("alert")] = r->alert();
    o[QLatin1StringView("open")] = r->open();
    o[QLatin1StringView("blocker")] = r->blocker();
    o[QLatin1StringView("blocked")] = r->blocked();
    o[QLatin1StringView("encrypted")] = r->encrypted();
    o[QLatin1StringView("archived")] = r->archived();
    o[QLatin1StringView("broadcast")] = r->broadcast();
    if (r->joinCodeRequired()) {
        o[QLatin1StringView("joinCodeRequired")] = true;
    }
    if (!r->e2EKey().isEmpty()) {
        o[QLatin1StringView("e2ekey")] = r->e2EKey();
    }
    if (!r->e2eKeyId().isEmpty()) {
        o[QLatin1StringView("e2ekeyid")] = r->e2eKeyId();
    }

    if (!r->description().isEmpty()) {
        o[QLatin1StringView("description")] = r->description();
    }
    o[QLatin1StringView("userMentions")] = r->userMentions();
    if (r->groupMentions() > 0) {
        o[QLatin1StringView("groupMentions")] = r->groupMentions();
    }

    serializeStringList(o, QLatin1StringView("mutedUsers"), r->mutedUsers());

    serializeStringList(o, QLatin1StringView("ignored"), r->ignoredUsers());
    serializeStringList(o, QLatin1StringView("tunread"), r->threadUnread());

    serializeStringList(o, QLatin1StringView("roles"), r->roles());

    o[QLatin1StringView("notifications")] = NotificationOptions::serialize(r->notificationOptions());

    if (!r->directChannelUserId().isEmpty()) {
        o[QLatin1StringView("directChannelUserId")] = QString::fromLatin1(r->directChannelUserId());
    }

    serializeStringList(o, QLatin1StringView("systemMessages"), r->displaySystemMessageTypes());

    serializeStringList(o, QLatin1StringView("userHighlights"), r->highlightsWord());

    if (!r->avatarETag().isEmpty()) {
        o[QLatin1StringView("avatarETag")] = r->avatarETag();
    }
    if (!r->uids().isEmpty()) {
        o[QLatin1StringView("uids")] = QJsonArray::fromStringList(r->uids());
    }

    if (r->retentionInfo().isNotDefault()) {
        o[QLatin1StringView("retention")] = RetentionInfo::serialize(r->retentionInfo());
    }
    if (r->teamInfo().isValid()) {
        TeamInfo::serialize(r->teamInfo(), o);
    }
    if (!r->parentRid().isEmpty()) {
        o[QLatin1StringView("prid")] = QString::fromLatin1(r->parentRid());
    }

    serializeStringList(o, QLatin1StringView("usernames"), r->userNames());

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

bool Room::encryptedEnabled() const
{
    if (mRocketChatAccount) {
        return mRocketChatAccount->encryptionEnabled();
    }
    return false;
}

bool Room::userIsIgnored(const QByteArray &userId)
{
    // TODO Convert mIgnoredUsers to QList<QByteArray>
    return mIgnoredUsers.contains(QString::fromLatin1(userId));
}

bool Room::roomIsBlocked() const
{
    return ((mReadOnly && !canChangeRoles()) || mArchived) || mBlocker || mBlocked;
}

QString Room::roomMessageInfo() const
{
    if ((mReadOnly && !canChangeRoles()) || mArchived) {
        return i18n("Channel is read only.");
    }
    if (mBlocker) {
        return i18n("You have blocked this channel.");
    }
    if (mBlocked) {
        return i18n("Channel was blocked.");
    }
    return {};
}

bool Room::canChangeRoles() const
{
    return mRoles.contains(QStringLiteral("owner"));
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

#include "moc_room.cpp"
