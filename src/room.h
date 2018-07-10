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

#ifndef ROOM_H
#define ROOM_H

#include <QJsonObject>
#include <QObject>
#include "notificationoptions.h"

#include "libruqola_private_export.h"
class UsersForRoomModel;
class UsersForRoomFilterProxyModel;
class FilesForRoomModel;
class FilesForRoomFilterProxyModel;
class MessageModel;
class RocketChatAccount;

class LIBRUQOLACORE_TESTS_EXPORT Room : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString announcement READ announcement WRITE setAnnouncement NOTIFY announcementChanged)
    Q_PROPERTY(QString topic READ topic WRITE setTopic NOTIFY topicChanged)
    Q_PROPERTY(bool favorite READ favorite WRITE setFavorite NOTIFY favoriteChanged)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(bool alert READ alert WRITE setAlert NOTIFY alertChanged)
    Q_PROPERTY(bool blocker READ blocker WRITE setBlocker NOTIFY blockerChanged)
    Q_PROPERTY(bool archived READ archived WRITE setArchived NOTIFY archivedChanged)
    Q_PROPERTY(bool blocked READ blocked WRITE setBlocked NOTIFY blockedChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QStringList roles READ roles WRITE setRoles NOTIFY rolesChanged)
public:
    explicit Room(RocketChatAccount *account = nullptr, QObject *parent = nullptr);

    // To be used in ID find: message ID
    Q_REQUIRED_RESULT bool operator==(const Room &other) const;

    //we can't use operator== as it tests only id. We need it for autotest
    Q_REQUIRED_RESULT bool isEqual(const Room &other) const;

    Q_REQUIRED_RESULT QString roomCreatorUserName() const;
    void setRoomCreatorUserName(const QString &userName);

    Q_REQUIRED_RESULT QString roomCreatorUserId() const;
    void setRoomCreatorUserId(const QString &userId);

    Q_REQUIRED_RESULT QStringList mutedUsers() const;
    void setMutedUsers(const QStringList &mutedUsers);

    Q_REQUIRED_RESULT qint64 jitsiTimeout() const;
    void setJitsiTimeout(const qint64 &jitsiTimeout);

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

    Q_REQUIRED_RESULT QString announcement() const;
    void setAnnouncement(const QString &announcement);

    Q_REQUIRED_RESULT QString channelType() const;
    void setChannelType(const QString &channelType);

    Q_REQUIRED_RESULT bool favorite() const;
    void setFavorite(bool favorite);

    QString topic() const;
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

    FilesForRoomModel *filesModelForRoom() const;
    FilesForRoomFilterProxyModel *filesForRoomFilterProxyModel() const;

    MessageModel *messageModel() const;

    QString inputMessage() const;
    void setInputMessage(const QString &inputMessage);

    Q_REQUIRED_RESULT bool archived() const;
    void setArchived(bool archived);

    QString description() const;
    void setDescription(const QString &description);

    Q_REQUIRED_RESULT bool canBeModify() const;
    Q_REQUIRED_RESULT NotificationOptions notificationOptions() const;
    void setNotificationOptions(const NotificationOptions &notificationOptions);

    Q_REQUIRED_RESULT int userMentions() const;
    void setUserMentions(int userMentions);

    void updateSubscriptionRoom(const QJsonObject &json);
    Q_REQUIRED_RESULT qint64 updatedAt() const;
    void setUpdatedAt(const qint64 &updatedAt);

    void parseInsertRoom(const QJsonObject &json);

    Q_REQUIRED_RESULT qint64 lastSeeAt() const;
    void setLastSeeAt(const qint64 &lastSeeAt);

    Q_REQUIRED_RESULT bool blocked() const;
    void setBlocked(bool blocked);

    Q_REQUIRED_RESULT QStringList roles() const;
    void setRoles(const QStringList &roles);

    Q_REQUIRED_RESULT QStringList ignoredUsers() const;
    void setIgnoredUsers(const QStringList &ignoredUsers);

Q_SIGNALS:
    void nameChanged();
    void announcementChanged();
    void topicChanged();
    void favoriteChanged();
    void alertChanged();
    void readOnlyChanged();
    void unreadChanged();

    //Blocker we blocked the channel
    void blockerChanged();
    //Blocked the channel was blocked
    void blockedChanged();

    void archivedChanged();
    void descriptionChanged();
    void rolesChanged();

private:
    Q_DISABLE_COPY(Room)
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

    // name
    QString mName;

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
    qint64 mLastSeeAt = -1;

    //quint64 ?
    int mUnread = 0;
    int mUserMentions = 0;

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
    UsersForRoomModel *mUsersModelForRoom = nullptr;
    UsersForRoomFilterProxyModel *mUsersModelForRoomProxyModel = nullptr;
    FilesForRoomModel *mFilesModelForRoom = nullptr;
    FilesForRoomFilterProxyModel *mFilesForRoomFilterProxyModel = nullptr;

    MessageModel *mMessageModel = nullptr;
    RocketChatAccount *mRocketChatAccount = nullptr;
};

LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Room &t);
#endif // ROOM_H
