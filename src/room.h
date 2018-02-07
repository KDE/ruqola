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

#ifndef ROOM_H
#define ROOM_H

#include <QJsonObject>
#include <QObject>

#include "libruqola_private_export.h"
class UsersForRoomModel;
class UsersForRoomFilterProxyModel;
class FilesForRoomModel;
class FilesForRoomFilterProxyModel;

class LIBRUQOLACORE_TESTS_EXPORT Room : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString announcement READ announcement WRITE setAnnouncement NOTIFY announcementChanged)
    Q_PROPERTY(QString topic READ topic WRITE setTopic NOTIFY topicChanged)
    Q_PROPERTY(bool favorite READ favorite WRITE setFavorite NOTIFY favoriteChanged)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(bool alert READ alert WRITE setAlert NOTIFY alertChanged)
public:
    explicit Room(QObject *parent = nullptr);

    // To be used in ID find: message ID
    bool operator==(const Room &other) const;

    //we can't use operator== as it tests only id. We need it for autotest
    bool isEqual(const Room &other) const;

    QString userName() const;
    void setUserName(const QString &userName);

    QString userId() const;
    void setUserId(const QString &userId);

    QStringList mutedUsers() const;
    void setMutedUsers(const QStringList &mutedUsers);

    qint64 jitsiTimeout() const;
    void setJitsiTimeout(const qint64 &jitsiTimeout);

    int unread() const;
    void setUnread(int unread);

    bool selected() const;
    void setSelected(bool selected);

    /**
    * @brief Return room name
    *
    * @return QString, The name of the room
    */
    QString name() const;
    void setName(const QString &name);

    QString announcement() const;
    void setAnnouncement(const QString &announcement);

    QString channelType() const;
    void setChannelType(const QString &channelType);

    bool favorite() const;
    void setFavorite(bool favorite);

    QString topic() const;
    void setTopic(const QString &topic);

    bool readOnly() const;
    void setReadOnly(bool readOnly);

    bool open() const;
    void setOpen(bool open);

    bool alert() const;
    void setAlert(bool alert);

    QString id() const;
    void setId(const QString &id);

    void parseSubscriptionRoom(const QJsonObject &json);
    void parseRoom(const QJsonObject &json);
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
    static QByteArray serialize(Room *r);

    Q_INVOKABLE UsersForRoomModel *usersModelForRoom() const;

    Q_INVOKABLE UsersForRoomFilterProxyModel *usersModelForRoomProxyModel() const;

    Q_INVOKABLE FilesForRoomModel *filesModelForRoom() const;

    FilesForRoomFilterProxyModel *filesForRoomFilterProxyModel() const;

Q_SIGNALS:
    void nameChanged();
    void announcementChanged();
    void topicChanged();
    void favoriteChanged();
    void userIdChanged();
    void alertChanged();
    void readOnlyChanged();
    void unreadChanged();

private:
    Q_DISABLE_COPY(Room)
    //Room Object Fields

    // _id
    QString mRoomId;

    // t (can take values "d" , "c" or "p")
    QString mChannelType;

    // name
    QString mName;

    // Announcement
    QString mAnnouncement;

    // u
    QString mUserName;
    QString mUserId;

    // topic
    QString mTopic;

    // muted - collection of muted users by its usernames
    QStringList mMutedUsers;

    // jitsiTimeout
    qint64 mJitsiTimeout = -1;

    // ro - read-only chat or not
    bool mReadOnly = false;

    //quint64 ?
    int mUnread = -1;
    bool mSelected = false;
    bool mFavorite = false;
    //We can hide it or not.
    bool mOpen = false;
    bool mAlert = false;
    UsersForRoomModel *mUsersModelForRoom = nullptr;
    UsersForRoomFilterProxyModel *mUsersModelForRoomProxyModel = nullptr;
    FilesForRoomModel *mFilesModelForRoom = nullptr;
    FilesForRoomFilterProxyModel *mFilesForRoomFilterProxyModel = nullptr;
};

LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Room &t);
#endif // ROOM_H
