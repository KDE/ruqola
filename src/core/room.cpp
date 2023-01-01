/*
 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "room.h"
#include "model/messagemodel.h"
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
    mMessageModel = new MessageModel(QString(), mRocketChatAccount, this, this);
}

Room::~Room() = default;

Room::RoomType Room::roomTypeFromString(const QString &type)
{
    if (type == QStringLiteral("p")) {
        return Room::RoomType::Private;
    } else if (type == QStringLiteral("c")) {
        return Room::RoomType::Channel;
    } else if (type == QStringLiteral("d")) {
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
        && (mTeamInfo == other.teamInfo()) && (mLastMessageAt == other.lastMessageAt());
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
    if (json.contains(QLatin1String("joinCodeRequired"))) {
        setJoinCodeRequired(json[QStringLiteral("joinCodeRequired")].toBool());
    } else {
        setJoinCodeRequired(false);
    }

    if (json.contains(QLatin1String("fname"))) {
        setFName(json[QStringLiteral("fname")].toString());
    }
    if (json.contains(QLatin1String("autoTranslateLanguage"))) {
        setAutoTranslateLanguage(json[QStringLiteral("autoTranslateLanguage")].toString());
    }
    if (json.contains(QLatin1String("autoTranslate"))) {
        setAutoTranslate(json[QStringLiteral("autoTranslate")].toBool());
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

    if (json.contains(QLatin1String("encrypted"))) {
        setEncrypted(json[QStringLiteral("encrypted")].toBool());
    } else {
        setEncrypted(false);
    }
    // TODO verify it. add autotest
    if (json.contains(QLatin1String("broadcast"))) {
        setBroadcast(json[QStringLiteral("broadcast")].toBool());
    } else {
        setBroadcast(false);
    }
    setReadOnly(json[QStringLiteral("ro")].toBool());
    const qint64 result = Utils::parseDate(QStringLiteral("ls"), json);
    if (result != -1) {
        setLastSeenAt(result);
    }
    const qint64 lm = Utils::parseDate(QStringLiteral("lm"), json);
    if (lm != -1) {
        setLastMessageAt(lm);
    }

    const QJsonArray highlightsWordArray = json.value(QLatin1String("userHighlights")).toArray();
    QStringList lstHighlightsWord;
    const int highlightsWordArrayCount = highlightsWordArray.count();
    lstHighlightsWord.reserve(highlightsWordArrayCount);
    for (int i = 0; i < highlightsWordArrayCount; ++i) {
        lstHighlightsWord << highlightsWordArray.at(i).toString();
    }
    setHighlightsWord(lstHighlightsWord);

    if (json.contains(QLatin1String("ignored"))) {
        const QJsonArray ignoredArray = json.value(QLatin1String("ignored")).toArray();
        QStringList lstIgnored;
        const int ignoredArrayCount = ignoredArray.count();
        lstIgnored.reserve(ignoredArrayCount);
        for (int i = 0; i < ignoredArrayCount; ++i) {
            lstIgnored << ignoredArray.at(i).toString();
        }
        setIgnoredUsers(lstIgnored);
    }

    // TODO muted ????
    // TODO E2EKey
    setE2eKeyId(json[QStringLiteral("e2eKeyId")].toString());

    const QJsonValue ownerValue = json.value(QLatin1String("u"));
    if (!ownerValue.isUndefined()) {
        const QJsonObject objOwner = ownerValue.toObject();
        setRoomCreatorUserId(objOwner.value(QLatin1String("_id")).toString());
        setRoomCreatorUserName(objOwner.value(QLatin1String("username")).toString());
    } else {
        // When room is initialized we are the owner. When we update room we have the real
        // owner and if it's empty => we need to clear it.
        setRoomCreatorUserId(QString());
        setRoomCreatorUserName(QString());
    }
    if (json.contains(QLatin1String("prid"))) {
        setParentRid(json[QStringLiteral("prid")].toString());
    }
    if (json.contains(QLatin1String("uids"))) {
        const QJsonArray &uids = json[QStringLiteral("uids")].toArray();
        const auto &u0 = uids[0].toString();
        const auto &u1 = uids[1].toString();
        setDirectChannelUserId((u0 == mRocketChatAccount->userId()) ? u1 : u0);
    }

    const QJsonArray uidsArray = json.value(QLatin1String("uids")).toArray();
    QStringList lstUids;
    lstUids.reserve(uidsArray.count());
    for (int i = 0; i < uidsArray.count(); ++i) {
        lstUids << uidsArray.at(i).toString();
    }
    setUids(lstUids);

    const QJsonArray userNamesArray = json.value(QLatin1String("usernames")).toArray();
    QStringList lstUserNames;
    lstUids.reserve(userNamesArray.count());
    for (int i = 0; i < userNamesArray.count(); ++i) {
        lstUserNames << userNamesArray.at(i).toString();
    }
    setUserNames(lstUserNames);

    setAvatarETag(json.value(QLatin1String("avatarETag")).toString());
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

QString Room::roomCreatorUserId() const
{
    return mRoomCreateUserId;
}

void Room::setRoomCreatorUserId(const QString &userId)
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

QString Room::roomId() const
{
    return mRoomId;
}

void Room::setRoomId(const QString &id)
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
    QString needUpdateMessageId;
    return TextConverter::convertMessageText(mTopic, {}, {}, {}, emojiManager, messageCache, needUpdateMessageId, {}, {});
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
    QString needUpdateMessageId;
    return TextConverter::convertMessageText(mAnnouncement, {}, {}, {}, emojiManager, messageCache, needUpdateMessageId, {}, {});
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
    const QString roomID = json.value(QLatin1String("_id")).toString();
    // qDebug() << " json " << json;
    setRoomId(roomID);
    setName(json[QStringLiteral("name")].toString());
    setFName(json[QStringLiteral("fname")].toString());
    setAutoTranslateLanguage(json[QStringLiteral("autoTranslateLanguage")].toString());
    setAutoTranslate(json[QStringLiteral("autoTranslate")].toBool());
    setJitsiTimeout(Utils::parseDate(QStringLiteral("jitsiTimeout"), json));
    // topic/announcement/description is not part of update subscription
    const QString roomType = json.value(QLatin1String("t")).toString();
    setChannelType(Room::roomTypeFromString(roomType));
    const QJsonValue favoriteValue = json.value(QLatin1String("f"));
    if (!favoriteValue.isUndefined()) {
        setFavorite(favoriteValue.toBool());
    }
    setReadOnly(json[QStringLiteral("ro")].toBool());
    if (json.contains(QLatin1String("userMentions"))) {
        setUserMentions(json[QStringLiteral("userMentions")].toInt());
    }
    if (json.contains(QLatin1String("announcement"))) {
        setAnnouncement(json[QStringLiteral("announcement")].toString());
    }
    if (json.contains(QLatin1String("description"))) {
        setDescription(json[QStringLiteral("description")].toString());
    }
    setUpdatedAt(Utils::parseDate(QStringLiteral("_updatedAt"), json));
    setLastSeenAt(Utils::parseDate(QStringLiteral("ls"), json));
    setLastMessageAt(Utils::parseDate(QStringLiteral("lm"), json));
    setUnread(json[QStringLiteral("unread")].toInt());
    setOpen(json[QStringLiteral("open")].toBool());
    setAlert(json[QStringLiteral("alert")].toBool());
    const QJsonValue blockerValue = json.value(QLatin1String("blocker"));
    if (!blockerValue.isUndefined()) {
        setBlocker(blockerValue.toBool());
    } else {
        setBlocker(false);
    }

    // setE2eKeyId(json[QStringLiteral("e2eKeyId")].toString());
    setE2EKey(json[QStringLiteral("E2EKey")].toString());

    if (json.contains(QLatin1String("encrypted"))) {
        setEncrypted(json[QStringLiteral("encrypted")].toBool());
    } else {
        setEncrypted(false);
    }

    // Blocked ???
    const QJsonValue archivedValue = json.value(QLatin1String("archived"));
    if (!archivedValue.isUndefined()) {
        setArchived(archivedValue.toBool());
    } else {
        setArchived(false);
    }

    parseCommonData(json);
    parseDisplaySystemMessage(json);

    const QJsonValue ownerValue = json.value(QLatin1String("u"));
    if (!ownerValue.isUndefined()) {
        const QJsonObject objOwner = ownerValue.toObject();
        setRoomCreatorUserId(objOwner.value(QLatin1String("_id")).toString());
        setRoomCreatorUserName(objOwner.value(QLatin1String("username")).toString());
    } else {
        // When room is initialized we are the owner. When we update room we have the real
        // owner and if it's empty => we need to clear it.
        setRoomCreatorUserId(QString());
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
    QString roomID = json.value(QLatin1String("rid")).toString();
    if (roomID.isEmpty()) {
        roomID = json.value(QLatin1String("_id")).toString();
    }
    setRoomId(roomID);
    setName(json[QStringLiteral("name")].toString());
    setFName(json[QStringLiteral("fname")].toString());
    setAutoTranslateLanguage(json[QStringLiteral("autoTranslateLanguage")].toString());
    setAutoTranslate(json[QStringLiteral("autoTranslate")].toBool());
    setJitsiTimeout(Utils::parseDate(QStringLiteral("jitsiTimeout"), json));
    // topic/announcement/description is not part of update subscription
    const QString roomType = json.value(QLatin1String("t")).toString();
    setChannelType(Room::roomTypeFromString(roomType));
    const QJsonValue favoriteValue = json.value(QLatin1String("f"));
    if (!favoriteValue.isUndefined()) {
        setFavorite(favoriteValue.toBool());
    }
    setE2EKey(json[QStringLiteral("E2EKey")].toString());
    setReadOnly(json[QStringLiteral("ro")].toBool());

    setUpdatedAt(Utils::parseDate(QStringLiteral("_updatedAt"), json));
    setLastSeenAt(Utils::parseDate(QStringLiteral("ls"), json));
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
    // TODO e2ekey
    // TODO blocked ?
    const QJsonValue archivedValue = json.value(QLatin1String("archived"));
    if (!archivedValue.isUndefined()) {
        setArchived(archivedValue.toBool());
    } else {
        setArchived(false);
    }

    parseCommonData(json);
    parseDisplaySystemMessage(json);

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
    // qDebug() << " *thus" << *this;
    mNotificationOptions.parseNotificationOptions(json);
    parseRetentionInfo(json);
    // parseTeamInfo(json);
    // TODO add muted
}

void Room::parseRetentionInfo(const QJsonObject &json)
{
    const QJsonValue retentionValue = json.value(QLatin1String("retention"));
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
    const QJsonArray sysMessArray = json.value(QLatin1String("sysMes")).toArray();
    QStringList lst;
    const auto sysMessArrayCount{sysMessArray.count()};
    lst.reserve(sysMessArrayCount);
    for (auto i = 0; i < sysMessArrayCount; ++i) {
        lst << sysMessArray.at(i).toString();
    }
    setDisplaySystemMessageTypes(lst);
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
        info.identifier = mRoomId;
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

    // FIXME.
    const QJsonArray highlightsWordArray = json.value(QLatin1String("userHighlights")).toArray();
    QStringList lstHighlightsWord;
    const int highlightsWordArrayCount = highlightsWordArray.count();
    lstHighlightsWord.reserve(highlightsWordArrayCount);
    for (int i = 0; i < highlightsWordArrayCount; ++i) {
        lstHighlightsWord << highlightsWordArray.at(i).toString();
    }
    setHighlightsWord(lstHighlightsWord);
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

QString Room::parentRid() const
{
    return mParentRid;
}

void Room::setParentRid(const QString &parentRid)
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

QStringList Room::rolesForUserId(const QString &userId)
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

std::unique_ptr<Room> Room::fromJSon(const QJsonObject &o)
{
    // FIXME
    auto r = std::make_unique<Room>(nullptr);

    r->setRoomId(o[QStringLiteral("rid")].toString());
    r->setChannelType(Room::roomTypeFromString(o[QStringLiteral("t")].toString()));
    r->setName(o[QStringLiteral("name")].toString());
    r->setFName(o[QStringLiteral("fname")].toString());
    r->setAutoTranslateLanguage(o[QStringLiteral("autoTranslateLanguage")].toString());
    r->setAutoTranslate(o[QStringLiteral("autoTranslate")].toBool());
    r->setRoomCreatorUserName(o[QStringLiteral("roomCreatorUserName")].toString());
    r->setRoomCreatorUserId(o[QStringLiteral("roomCreatorUserID")].toString());
    r->setTopic(o[QStringLiteral("topic")].toString());
    r->setJitsiTimeout(static_cast<qint64>(o[QStringLiteral("jitsiTimeout")].toDouble()));
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
    r->setEncrypted(o[QStringLiteral("encrypted")].toBool());
    r->setBroadcast(o[QStringLiteral("broadcast")].toBool());
    r->setE2EKey(o[QStringLiteral("e2ekey")].toString());
    r->setE2eKeyId(o[QStringLiteral("e2ekeyid")].toString());
    r->setJoinCodeRequired(o[QStringLiteral("joinCodeRequired")].toBool());
    r->setUpdatedAt(static_cast<qint64>(o[QStringLiteral("updatedAt")].toDouble()));
    r->setLastSeenAt(static_cast<qint64>(o[QStringLiteral("lastSeenAt")].toDouble()));
    const QJsonArray mutedArray = o.value(QLatin1String("mutedUsers")).toArray();
    QStringList lst;
    lst.reserve(mutedArray.count());
    for (int i = 0; i < mutedArray.count(); ++i) {
        lst << mutedArray.at(i).toString();
    }
    r->setMutedUsers(lst);

    const QJsonArray systemMessagesArray = o.value(QLatin1String("systemMessages")).toArray();
    lst.clear();
    lst.reserve(systemMessagesArray.count());
    for (int i = 0; i < systemMessagesArray.count(); ++i) {
        lst << systemMessagesArray.at(i).toString();
    }
    r->setDisplaySystemMessageTypes(lst);

    const QJsonArray ignoredArray = o.value(QLatin1String("ignored")).toArray();
    QStringList lstIgnored;
    const int ignoredArrayCount = ignoredArray.count();
    lstIgnored.reserve(ignoredArrayCount);
    for (int i = 0; i < ignoredArrayCount; ++i) {
        lstIgnored << ignoredArray.at(i).toString();
    }
    r->setIgnoredUsers(lstIgnored);

    const QJsonArray highlightsWordArray = o.value(QLatin1String("userHighlights")).toArray();
    QStringList lstHighlightsWord;
    const int highlightsWordArrayCount = highlightsWordArray.count();
    lstHighlightsWord.reserve(highlightsWordArrayCount);
    for (int i = 0; i < highlightsWordArrayCount; ++i) {
        lstHighlightsWord << highlightsWordArray.at(i).toString();
    }
    r->setHighlightsWord(lstHighlightsWord);

    const QJsonArray rolesArray = o.value(QLatin1String("roles")).toArray();
    QStringList lstRoles;
    lstRoles.reserve(rolesArray.count());
    for (int i = 0; i < rolesArray.count(); ++i) {
        lstRoles << rolesArray.at(i).toString();
    }
    r->setRoles(lstRoles);

    const QJsonObject notificationsObj = o.value(QLatin1String("notifications")).toObject();
    const NotificationOptions notifications = NotificationOptions::fromJSon(notificationsObj);
    r->setNotificationOptions(notifications);

    r->setDirectChannelUserId(o[QStringLiteral("directChannelUserId")].toString());

    r->setAvatarETag(o[QStringLiteral("avatarETag")].toString());

    const QJsonArray uidsArray = o.value(QLatin1String("uids")).toArray();
    QStringList lstUids;
    lstUids.reserve(uidsArray.count());
    for (int i = 0; i < uidsArray.count(); ++i) {
        lstUids << uidsArray.at(i).toString();
    }
    r->setUids(lstUids);
    const QJsonObject retentionObj = o.value(QLatin1String("retention")).toObject();
    const RetentionInfo retention = RetentionInfo::fromJSon(retentionObj);
    r->setRetentionInfo(retention);
    const TeamInfo teaminfo = TeamInfo::fromJSon(o);
    r->setTeamInfo(teaminfo);

    // TODO add parent RID

    return r;
}

QByteArray Room::serialize(Room *r, bool toBinary)
{
    QJsonDocument d;
    QJsonObject o;

    // todo add timestamp

    o[QStringLiteral("rid")] = r->roomId();
    o[QStringLiteral("t")] = Room::roomFromRoomType(r->channelType());
    o[QStringLiteral("name")] = r->name();
    o[QStringLiteral("fname")] = r->fName();
    o[QStringLiteral("roomCreatorUserName")] = r->roomOwnerUserName();
    o[QStringLiteral("roomCreatorUserID")] = r->roomCreatorUserId();
    if (!r->topic().isEmpty()) {
        o[QStringLiteral("topic")] = r->topic();
    }
    if (!r->autoTranslateLanguage().isEmpty()) {
        o[QStringLiteral("autoTranslateLanguage")] = r->autoTranslateLanguage();
    }
    if (r->autoTranslate()) {
        o[QStringLiteral("autoTranslate")] = r->autoTranslate();
    }
    o[QStringLiteral("jitsiTimeout")] = r->jitsiTimeout();
    o[QStringLiteral("updatedAt")] = r->updatedAt();
    o[QStringLiteral("lastSeenAt")] = r->lastSeenAt();
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
    o[QStringLiteral("encrypted")] = r->encrypted();
    o[QStringLiteral("archived")] = r->archived();
    o[QStringLiteral("broadcast")] = r->broadcast();
    if (r->joinCodeRequired()) {
        o[QStringLiteral("joinCodeRequired")] = true;
    }
    if (!r->e2EKey().isEmpty()) {
        o[QStringLiteral("e2ekey")] = r->e2EKey();
    }
    if (!r->e2eKeyId().isEmpty()) {
        o[QStringLiteral("e2ekeyid")] = r->e2eKeyId();
    }

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

    if (!r->directChannelUserId().isEmpty()) {
        o[QStringLiteral("directChannelUserId")] = r->directChannelUserId();
    }

    if (!r->displaySystemMessageTypes().isEmpty()) {
        QJsonArray array;
        const int nbDisplaySystemMessageType = r->displaySystemMessageTypes().count();
        for (int i = 0; i < nbDisplaySystemMessageType; ++i) {
            array.append(r->mutedUsers().at(i));
        }
        o[QStringLiteral("systemMessages")] = array;
    }

    if (!r->avatarETag().isEmpty()) {
        o[QStringLiteral("avatarETag")] = r->avatarETag();
    }
    if (!r->uids().isEmpty()) {
        o[QStringLiteral("uids")] = QJsonArray::fromStringList(r->uids());
    }

    if (r->retentionInfo().isNotDefault()) {
        o[QStringLiteral("retention")] = RetentionInfo::serialize(r->retentionInfo());
    }
    if (r->teamInfo().isValid()) {
        TeamInfo::serialize(r->teamInfo(), o);
    }

    if (toBinary) {
        return QCborValue::fromJsonValue(o).toCbor();
    }
    d.setObject(o);
    return d.toJson(QJsonDocument::Indented);
}

UsersForRoomModel *Room::usersModelForRoom() const
{
    return mUsersModelForRoom;
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

bool Room::encryptedEnabled() const
{
    if (mRocketChatAccount) {
        return mRocketChatAccount->encryptionEnabled();
    }
    return false;
}

bool Room::userIsIgnored(const QString &userId)
{
    return mIgnoredUsers.contains(userId);
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

bool Room::userHasOwnerRole(const QString &userId) const
{
    const Role r = mRolesForRooms.findRoleByUserId(userId);
    if (r.isValid()) {
        return r.isOwner();
    }
    return false;
}

bool Room::userHasLeaderRole(const QString &userId) const
{
    const Role r = mRolesForRooms.findRoleByUserId(userId);
    if (r.isValid()) {
        return r.isLeader();
    }
    return false;
}

bool Room::userHasModeratorRole(const QString &userId) const
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

QString Room::directChannelUserId() const
{
    return mDirectChannelUserId;
}

void Room::setDirectChannelUserId(const QString &uid)
{
    if (mDirectChannelUserId != uid) {
        mDirectChannelUserId = uid;
        Q_EMIT directChannelUserIdChanged();
    }
}

bool Room::roomIsEmpty() const
{
    if (mMessageModel) {
        return (mMessageModel->rowCount() == 0);
    }
    return true;
}
