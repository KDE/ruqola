/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright 2017-2020 Laurent Montel <montel@kde.org>
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

#ifndef ROOM_H
#define ROOM_H

#include <QJsonObject>
#include <QObject>
#include "notificationoptions.h"
#include "roles.h"

#include "libruqola_private_export.h"
class UsersForRoomModel;
class UsersForRoomFilterProxyModel;
class MessageModel;
class RocketChatAccount;
class NotificationOptionsWrapper;
class LIBRUQOLACORE_TESTS_EXPORT Room : public QObject
{
    Q_OBJECT
public:
    explicit Room(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    ~Room() override;

    // To be used in ID find: message ID
    Q_REQUIRED_RESULT bool operator==(const Room &other) const;

    //we can't use operator== as it tests only id. We need it for autotest
    Q_REQUIRED_RESULT bool isEqual(const Room &other) const;

    Q_REQUIRED_RESULT QString roomOwnerUserName() const;
    void setRoomCreatorUserName(const QString &userName);

    Q_REQUIRED_RESULT QString roomCreatorUserId() const;
    void setRoomCreatorUserId(const QString &userId);

    Q_REQUIRED_RESULT QStringList mutedUsers() const;
    void setMutedUsers(const QStringList &mutedUsers);

    Q_REQUIRED_RESULT qint64 jitsiTimeout() const;
    void setJitsiTimeout(qint64 jitsiTimeout);

    Q_REQUIRED_RESULT int unread() const;
    void setUnread(int unread);

    Q_REQUIRED_RESULT bool selected() const;
    void setSelected(bool selected);

    /**
    * @brief Return room name
    *
    * @return QString, The name of the room
    */
    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &name);

    Q_REQUIRED_RESULT QString displayRoomName() const;

    Q_REQUIRED_RESULT QString announcement() const;
    void setAnnouncement(const QString &announcement);

    Q_REQUIRED_RESULT QString channelType() const;
    void setChannelType(const QString &channelType);

    Q_REQUIRED_RESULT bool favorite() const;
    void setFavorite(bool favorite);

    Q_REQUIRED_RESULT QString topic() const;
    void setTopic(const QString &topic);

    Q_REQUIRED_RESULT bool readOnly() const;
    void setReadOnly(bool readOnly);

    Q_REQUIRED_RESULT bool open() const;
    void setOpen(bool open);

    Q_REQUIRED_RESULT bool alert() const;
    void setAlert(bool alert);

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &id);

    void setBlocker(bool alert);
    Q_REQUIRED_RESULT bool blocker() const;

    void parseSubscriptionRoom(const QJsonObject &json);
    void parseUpdateRoom(const QJsonObject &json);

    /**
    * @brief Constructs Room object from QJsonObject (cache)
    *
    * @param source The Json containing room attributes
    * @return Room object, The room constructed from Json
    */
    static Room *fromJSon(const QJsonObject &source);

    /**
    * @brief Constructs QBytearray from Room object
    *
    * @param message The Room object
    * @return QByteArray, The Json containing room attributes
    */
    static QByteArray serialize(Room *r, bool toBinary = true);

    UsersForRoomModel *usersModelForRoom() const;
    UsersForRoomFilterProxyModel *usersModelForRoomProxyModel() const;

    MessageModel *messageModel() const;

    Q_REQUIRED_RESULT QString inputMessage() const;
    void setInputMessage(const QString &inputMessage);

    Q_REQUIRED_RESULT bool archived() const;
    void setArchived(bool archived);

    Q_REQUIRED_RESULT QString description() const;
    void setDescription(const QString &description);

    Q_REQUIRED_RESULT bool encryptedEnabled() const;
    Q_REQUIRED_RESULT bool canBeModify() const;
    Q_REQUIRED_RESULT NotificationOptions notificationOptions() const;
    void setNotificationOptions(const NotificationOptions &notificationOptions);

    Q_REQUIRED_RESULT int userMentions() const;
    void setUserMentions(int userMentions);

    void updateSubscriptionRoom(const QJsonObject &json);
    Q_REQUIRED_RESULT qint64 updatedAt() const;
    void setUpdatedAt(qint64 updatedAt);

    void parseInsertRoom(const QJsonObject &json);

    Q_REQUIRED_RESULT qint64 lastSeenAt() const;
    void setLastSeenAt(qint64 lastSeenAt);

    Q_REQUIRED_RESULT bool blocked() const;
    void setBlocked(bool blocked);

    Q_REQUIRED_RESULT QStringList roles() const;
    void setRoles(const QStringList &roles);

    Q_REQUIRED_RESULT QStringList ignoredUsers() const;
    void setIgnoredUsers(const QStringList &ignoredUsers);

    Q_REQUIRED_RESULT bool encrypted() const;
    void setEncrypted(bool encrypted);

    Q_REQUIRED_RESULT bool userIsIgnored(const QString &userId);
    Q_REQUIRED_RESULT QString markdownAnnouncement() const;
    Q_REQUIRED_RESULT QString roomMessageInfo() const;

    Q_REQUIRED_RESULT QString e2EKey() const;
    void setE2EKey(const QString &e2EKey);

    Q_REQUIRED_RESULT QString e2eKeyId() const;
    void setE2eKeyId(const QString &e2eKeyId);

    NotificationOptionsWrapper *getNotificationWrapper();
    Q_REQUIRED_RESULT bool joinCodeRequired() const;
    void setJoinCodeRequired(bool joinCodeRequired);

    Q_REQUIRED_RESULT bool wasInitialized() const;
    void setWasInitialized(bool wasInitialized);

    Q_REQUIRED_RESULT Roles rolesForRooms() const;
    void setRolesForRooms(const Roles &rolesForRooms);

    Q_REQUIRED_RESULT QStringList rolesForUserId(const QString &userId);

    Q_REQUIRED_RESULT bool canChangeRoles() const;
    Q_REQUIRED_RESULT bool userHasOwnerRole(const QString &userId) const;
    Q_REQUIRED_RESULT bool userHasLeaderRole(const QString &userId) const;
    Q_REQUIRED_RESULT bool userHasModeratorRole(const QString &userId) const;

    void updateRoles(const QJsonObject &obj);

    Q_REQUIRED_RESULT bool broadcast() const;
    void setBroadcast(bool broadcast);

    Q_REQUIRED_RESULT QString parentRid() const;
    void setParentRid(const QString &parentRid);

    Q_REQUIRED_RESULT QString fName() const;
    void setFName(const QString &value);

    Q_REQUIRED_RESULT QString displayFName() const;
    Q_REQUIRED_RESULT bool isDiscussionRoom() const;

    Q_REQUIRED_RESULT QString autoTranslateLanguage() const;
    void setAutoTranslateLanguage(const QString &autoTranslateLanguage);

    Q_REQUIRED_RESULT bool autoTranslate() const;
    void setAutoTranslate(bool autoTranslate);

    Q_REQUIRED_RESULT QString directChannelUserId() const;
    void setDirectChannelUserId(const QString &userId);

    Q_REQUIRED_RESULT QStringList displaySystemMessageTypes() const;
    void setDisplaySystemMessageTypes(const QStringList &displaySystemMessageTypes);

Q_SIGNALS:
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

    //Blocker we blocked the channel
    void blockerChanged();
    //Blocked the channel was blocked
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

    void directChannelUserIdChanged();
    void displaySystemMessageTypesChanged();

private:
    Q_DISABLE_COPY(Room)
    void parseCommonData(const QJsonObject &json);
    void parseDisplaySystemMessage(const QJsonObject &json);

    //Room Object Fields

    NotificationOptions mNotificationOptions;
    // muted - collection of muted users by its usernames
    QStringList mMutedUsers;

    QStringList mIgnoredUsers;

    //Roles
    QStringList mRoles;

    QString mInputMessage;

    // _id
    QString mRoomId;

    // t (can take values "d" , "c" or "p" or "l")
    QString mChannelType;

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

    //Encryption Key
    QString mE2EKey;
    QString mE2eKeyId;

    //Autotranslate
    QString mAutotranslateLanguage;

    //Roles In Room
    Roles mRolesForRooms;

    //quint64 ?
    int mUnread = 0;
    int mUserMentions = 0;

    int mThreadCount = 0;

    // ro - read-only chat or not
    bool mReadOnly = false;
    bool mSelected = false;
    bool mFavorite = false;
    //We can hide it or not.
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

    UsersForRoomModel *mUsersModelForRoom = nullptr;
    UsersForRoomFilterProxyModel *mUsersModelForRoomProxyModel = nullptr;
    MessageModel *mMessageModel = nullptr;
    RocketChatAccount *mRocketChatAccount = nullptr;
};

LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Room &t);
#endif // ROOM_H
