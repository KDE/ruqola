/*
 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "notifications/notificationoptions.h"
#include "roles/roles.h"
#include <QJsonObject>
#include <QObject>
#include <QPointer>

#include "channelcounterinfo.h"
#include "libruqolacore_export.h"
#include "retentioninfo.h"
#include "teams/teaminfo.h"
#include "utils.h"

#include <memory>

class UsersForRoomModel;
class MessagesModel;
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT Room : public QObject
{
    Q_OBJECT
public:
    explicit Room(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    ~Room() override;

    struct TeamRoomInfo {
        QString teamName;
        QString teamIdentifier;
    };

    // t (can take values "d" , "c" or "p" or "l")
    enum class RoomType {
        Unknown,
        Direct,
        Channel,
        Private,
    };
    Q_ENUM(RoomType)

    [[nodiscard]] static QString roomFromRoomType(RoomType type);

    // To be used in ID find: message ID
    [[nodiscard]] bool operator==(const Room &other) const;

    // we can't use operator== as it tests only id. We need it for autotest
    [[nodiscard]] bool isEqual(const Room &other) const;

    [[nodiscard]] QString roomOwnerUserName() const;
    void setRoomCreatorUserName(const QString &userName);

    [[nodiscard]] QString roomCreatorUserId() const;
    void setRoomCreatorUserId(const QString &userId);

    [[nodiscard]] QStringList mutedUsers() const;
    void setMutedUsers(const QStringList &mutedUsers);

    [[nodiscard]] qint64 jitsiTimeout() const;
    void setJitsiTimeout(qint64 jitsiTimeout);

    [[nodiscard]] int unread() const;
    void setUnread(int unread);

    [[nodiscard]] bool selected() const;
    void setSelected(bool selected);

    /**
     * @brief Return room name
     *
     * @return QString, The name of the room
     */
    [[nodiscard]] QString name() const;
    void setName(const QString &name);

    [[nodiscard]] QString displayRoomName() const;

    [[nodiscard]] QString announcement() const;
    void setAnnouncement(const QString &announcement);

    [[nodiscard]] RoomType channelType() const;
    void setChannelType(RoomType channelType);

    [[nodiscard]] bool favorite() const;
    void setFavorite(bool favorite);

    [[nodiscard]] QString topic() const;
    void setTopic(const QString &topic);

    [[nodiscard]] bool readOnly() const;
    void setReadOnly(bool readOnly);

    [[nodiscard]] bool open() const;
    void setOpen(bool open);

    [[nodiscard]] bool alert() const;
    void setAlert(bool alert);

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &id);

    void setBlocker(bool alert);
    [[nodiscard]] bool blocker() const;

    void parseSubscriptionRoom(const QJsonObject &json);
    void parseUpdateRoom(const QJsonObject &json);

    /**
     * @brief Constructs Room object from QJsonObject (cache)
     *
     * @param source The Json containing room attributes
     * @return Room object, The room constructed from Json
     */
    static std::unique_ptr<Room> deserialize(const QJsonObject &source);
    static void deserialize(Room *r, const QJsonObject &o);

    /**
     * @brief Constructs QBytearray from Room object
     *
     * @param message The Room object
     * @return QByteArray, The Json containing room attributes
     */
    static QByteArray serialize(Room *r, bool toBinary = true);

    [[nodiscard]] UsersForRoomModel *usersModelForRoom() const;

    [[nodiscard]] MessagesModel *messageModel() const;

    [[nodiscard]] QString inputMessage() const;
    void setInputMessage(const QString &inputMessage);

    [[nodiscard]] bool archived() const;
    void setArchived(bool archived);

    [[nodiscard]] QString description() const;
    void setDescription(const QString &description);

    [[nodiscard]] bool encryptedEnabled() const;
    [[nodiscard]] bool canBeModify() const;
    [[nodiscard]] NotificationOptions notificationOptions() const;
    void setNotificationOptions(const NotificationOptions &notificationOptions);

    [[nodiscard]] int userMentions() const;
    void setUserMentions(int userMentions);

    void updateSubscriptionRoom(const QJsonObject &json);
    [[nodiscard]] qint64 updatedAt() const;
    void setUpdatedAt(qint64 updatedAt);

    void parseInsertRoom(const QJsonObject &json);

    [[nodiscard]] qint64 lastSeenAt() const;
    void setLastSeenAt(qint64 lastSeenAt);

    [[nodiscard]] qint64 lastMessageAt() const;
    void setLastMessageAt(qint64 lastMessageAt);

    [[nodiscard]] bool blocked() const;
    void setBlocked(bool blocked);

    [[nodiscard]] QStringList roles() const;
    void setRoles(const QStringList &roles);

    [[nodiscard]] QStringList ignoredUsers() const;
    void setIgnoredUsers(const QStringList &ignoredUsers);

    [[nodiscard]] bool encrypted() const;
    void setEncrypted(bool encrypted);

    [[nodiscard]] bool userIsIgnored(const QString &userId);
    [[nodiscard]] QString roomMessageInfo() const;

    [[nodiscard]] QString e2EKey() const;
    void setE2EKey(const QString &e2EKey);

    [[nodiscard]] QString e2eKeyId() const;
    void setE2eKeyId(const QString &e2eKeyId);

    [[nodiscard]] bool joinCodeRequired() const;
    void setJoinCodeRequired(bool joinCodeRequired);

    [[nodiscard]] bool wasInitialized() const;
    void setWasInitialized(bool wasInitialized);

    [[nodiscard]] Roles rolesForRooms() const;
    void setRolesForRooms(const Roles &rolesForRooms);

    [[nodiscard]] QStringList rolesForUserId(const QString &userId);

    [[nodiscard]] bool canChangeRoles() const;
    [[nodiscard]] bool userHasOwnerRole(const QString &userId) const;
    [[nodiscard]] bool userHasLeaderRole(const QString &userId) const;
    [[nodiscard]] bool userHasModeratorRole(const QString &userId) const;

    void updateRoles(const QJsonObject &obj);

    [[nodiscard]] bool broadcast() const;
    void setBroadcast(bool broadcast);

    [[nodiscard]] QString parentRid() const;
    void setParentRid(const QString &parentRid);

    [[nodiscard]] QString fName() const;
    void setFName(const QString &value);

    [[nodiscard]] QString displayFName() const;
    [[nodiscard]] bool isDiscussionRoom() const;

    [[nodiscard]] QString autoTranslateLanguage() const;
    void setAutoTranslateLanguage(const QString &autoTranslateLanguage);

    [[nodiscard]] bool autoTranslate() const;
    void setAutoTranslate(bool autoTranslate);

    [[nodiscard]] QString directChannelUserId() const;
    void setDirectChannelUserId(const QString &userId);

    [[nodiscard]] QStringList displaySystemMessageTypes() const;
    void setDisplaySystemMessageTypes(const QStringList &displaySystemMessageTypes);

    [[nodiscard]] ChannelCounterInfo channelCounterInfo() const;
    void setChannelCounterInfo(const ChannelCounterInfo &channelCounterInfo);

    void newMessageAdded();

    [[nodiscard]] bool allowToPinMessage() const;
    [[nodiscard]] QString avatarETag() const;
    void setAvatarETag(const QString &avatarETag);

    [[nodiscard]] Utils::AvatarInfo avatarInfo() const;

    [[nodiscard]] QStringList uids() const;
    void setUids(const QStringList &uids);

    [[nodiscard]] QStringList userNames() const;
    void setUserNames(const QStringList &userNames);

    [[nodiscard]] QStringList highlightsWord() const;
    void setHighlightsWord(const QStringList &highlightsWord);
    [[nodiscard]] bool hasPermission(const QString &permission) const;

    [[nodiscard]] QString displayTopic() const;
    [[nodiscard]] QString displayAnnouncement() const;

    [[nodiscard]] RetentionInfo retentionInfo() const;
    void setRetentionInfo(RetentionInfo retentionInfo);

    [[nodiscard]] static Room::RoomType roomTypeFromString(const QString &type);
    [[nodiscard]] TeamInfo teamInfo() const;
    void setTeamInfo(const TeamInfo &teamInfo);

    [[nodiscard]] Room::TeamRoomInfo teamRoomInfo() const;
    [[nodiscard]] bool roomIsBlocked() const;
    [[nodiscard]] bool hideBadgeForMention() const;
    [[nodiscard]] bool hideUnreadStatus() const;

    [[nodiscard]] qint64 numberMessages() const;
    void setNumberMessages(qint64 newNumberMessages);

    [[nodiscard]] int groupMentions() const;
    void setGroupMentions(int newGroupMentions);

Q_SIGNALS:
    void highlightsWordChanged();
    void nameChanged();
    void fnameChanged();
    void announcementChanged();
    void topicChanged();
    void favoriteChanged();
    void alertChanged();
    void readOnlyChanged();
    void unreadChanged();
    void openChanged();
    void encryptedChanged();
    void encryptionKeyChanged();
    void encryptionKeyIdChanged();
    void needAttention();

    // Blocker we blocked the channel
    void blockerChanged();
    // Blocked the channel was blocked
    void blockedChanged();

    void archivedChanged();
    void descriptionChanged();
    void rolesChanged();
    void mutedUsersChanged();
    void ignoredUsersChanged();
    void jitsiTimeoutChanged();
    void joinCodeRequiredChanged();
    void channelTypeChanged();

    void broadcastChanged();
    void parentRidChanged();
    void autoTranslateLanguageChanged();
    void autoTranslateChanged();
    void lastSeenChanged();
    void lastMessageAtChanged();

    void directChannelUserIdChanged();
    void displaySystemMessageTypesChanged();
    void channelCounterInfoChanged();

    void notificationOptionsChanged();
    void avatarETagChanged();

    void uidsChanged();
    void userNamesChanged();

    void retentionInfoChanged();
    void teamInfoChanged();

private:
    Q_DISABLE_COPY(Room)
    LIBRUQOLACORE_NO_EXPORT void parseCommonData(const QJsonObject &json);
    LIBRUQOLACORE_NO_EXPORT void parseDisplaySystemMessage(const QJsonObject &json);
    LIBRUQOLACORE_NO_EXPORT void parseRetentionInfo(const QJsonObject &json);
    LIBRUQOLACORE_NO_EXPORT void parseTeamInfo(const QJsonObject &obj);

    // Room Object Fields

    NotificationOptions mNotificationOptions;
    // muted - collection of muted users by its usernames
    QStringList mMutedUsers;

    QStringList mIgnoredUsers;

    QStringList mHighlightsWord;

    // Roles
    QStringList mRoles;

    QStringList mUids;
    QStringList mUserNames;

    QString mInputMessage;

    // _id
    QString mRoomId;

    // t (can take values "d" , "c" or "p" or "l")
    Room::RoomType mChannelType = RoomType::Unknown;

    // Parent Rid when we have a discussion.
    QString mParentRid;

    // name
    QString mName;

    QString mFName;

    // Announcement
    QString mAnnouncement;

    // u
    QString mRoomCreatorUserName;
    QString mRoomCreateUserId;

    // topic
    QString mTopic;

    QString mDescription;

    // jitsiTimeout
    qint64 mJitsiTimeout = -1;
    qint64 mUpdatedAt = -1;
    qint64 mLastSeenAt = -1;
    qint64 mLastMessageAt = -1;

    // Encryption Key
    QString mE2EKey;
    QString mE2eKeyId;

    // Autotranslate
    QString mAutotranslateLanguage;

    // AvatarEtag
    QString mAvatarETag;

    // Roles In Room
    Roles mRolesForRooms;

    // quint64 ?
    int mUnread = 0;
    int mUserMentions = 0;
    int mGroupMentions = 0;

    qint64 mNumberMessages = 0;

    // ro - read-only chat or not
    bool mReadOnly = false;
    bool mSelected = false;
    bool mFavorite = false;
    // We can hide it or not.
    bool mOpen = false;
    bool mAlert = false;
    bool mBlocker = false;
    bool mArchived = false;
    bool mBlocked = false;
    bool mEncrypted = false;
    bool mJoinCodeRequired = false;
    bool mWasInitialized = false;
    bool mBroadcast = false;
    bool mAutoTranslate = false;

    QString mDirectChannelUserId;

    QStringList mDisplaySystemMessageType;
    ChannelCounterInfo mChannelCounterInfo;
    RetentionInfo mRetentionInfo;
    TeamInfo mTeamInfo;
    mutable Utils::AvatarInfo mCurrentAvatarInfo;

    UsersForRoomModel *const mUsersModelForRoom;
    QPointer<MessagesModel> mMessageModel;
    RocketChatAccount *const mRocketChatAccount;
};

LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Room &t);
