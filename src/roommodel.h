/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
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

#ifndef ROOMMODEL_H
#define ROOMMODEL_H

#include <QAbstractListModel>
#include <QObject>


class Room
{
public:

    // To be used in ID find: message ID
    inline bool operator==(const Room &other) const
    {
        return other.id == id;
    }

    // To be used in sorted insert: timestamp
    inline bool operator<(const Room &other) const
    {
        return name < other.name;
    }

    /**
    * @brief Return room name
    *
    * @return QString, The name of the room
    */
    QString getName() const
    {
        return name;
    }

    /**
    * @brief Return topic name
    *
    * @return QString, The name of the topic of room
    */
    QString getTopic() const
    {
        return topic;
    }

    //Room Object Fields

    // _id
    QString id;

    // t (can take values "d" , "c" or "p")
    QString type;

    // name
    QString name;

    // u
    QString userName;
    QString userID;

    // topic
    QString topic;

    // muted - collection of muted users by its usernames
    QString mutedUsers; //QStringList

    // jitsiTimeout
    qint64 jitsiTimeout;

    // ro - read-only chat or not
    bool ro;

    int unread;
    bool selected = false;
};

class RoomWrapper : public QObject
{
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString topic READ getTopic NOTIFY topicChanged)
    Q_OBJECT

public:
    RoomWrapper(QObject *parent = 0);
    RoomWrapper(const Room &r, QObject *parent = 0);

    QString getName()
    {
        return m_name;
    }
    QString getTopic()
    {
        return m_topic;
    }

signals:
    void nameChanged();
    void topicChanged();

private:
    QString m_name, m_topic, m_id;
    int m_unread;
    bool m_selected;
};


class RoomModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum RoomRoles {
        RoomName = Qt::UserRole + 1,
        RoomSelected,
        RoomID,
        RoomUnread,
        RoomType,
        RoomUserName, //created by UserName
        RoomUserID,
        RoomTopic,
        RoomMuted,
        RoomJitsiTimeout,
        RoomRO
    };

    RoomModel(QObject *parent = 0);
    virtual ~RoomModel();

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

//     void setCurrentRoom(const QString &newRoom);
//     QString getCurrentRoom() const;

    /**
    * @brief Constructs room object from @param roomID and @param roomName and @param selected, then calls @method addRoom
    *
    * @param roomID The unique room ID
    * @param roomName The name of the room
    * @param selected True if room if @param roomID is selected, else false
    */
    Q_INVOKABLE void addRoom(const QString& roomID, const QString& roomName, bool selected = false);

    /**
    * @brief Adds a room to m_roomsList with @param room
    *
    * @param room The room to be added
    */
    void addRoom(const Room& room);

    /**
    * @brief Finds a room with @param roomID in m_roomsList
    *
    * @param roomID The ID of the room to find
    * @return RoomWrapper Pointer, The pointer to room with @param roomID in m_roomsList, if exists. Else return a new RoomWrapper object
    */
    RoomWrapper* findRoom(const QString &roomID) const;

    /**
    * @brief Constructs Message object from QJsonObject
    *
    * @param source The Json containing room attributes
    * @return Room object, The room constructed from Json
    */
    static Room fromJSon(const QJsonObject &source);

    /**
    * @brief Constructs QBytearray from Message object
    *
    * @param message The Room object
    * @return QByteArray, The Json containing room attributes
    */
    static QByteArray serialize(const Room &r);

    //void setActiveRoom(const QString &activeRoom);

    //Clear data and refill it with data in the cache, if there is
    void reset();
    void clear();
protected:
    virtual QHash<int, QByteArray> roleNames() const;

private:

    QVector<Room> m_roomsList;
    //QHash< QString, Room > m_roomsHash;
};

#endif // ROOMMODEL_H
