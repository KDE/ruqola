/*

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
#include "libruqolacore_export.h"
#include "room.h"
#include <QAbstractListModel>
#include <QObject>
class RoomWrapper;
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT RoomModel : public QAbstractListModel
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
        RoomMutedUsers,
        RoomJitsiTimeout,
        RoomRo,
        RoomAnnoucement,
        RoomOpen,
        RoomAlert,
        RoomOrder,
        RoomFavorite,
        RoomSection,
        RoomStatus,
        RoomIcon,
    };

    explicit RoomModel(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    virtual ~RoomModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
    * @brief Constructs room object from @param roomID and @param roomName and @param selected, then calls @method addRoom
    *
    * @param roomID The unique room ID
    * @param roomName The name of the room
    * @param selected True if room if @param roomID is selected, else false
    */
    Q_INVOKABLE void addRoom(const QString &roomID, const QString &roomName, bool selected = false);


    void updateRoom(const QString &name, const QString &roomID, const QString &topic, const QString &announcement, bool readOnly);

    /**
    * @brief Finds a room with @param roomID in m_roomsList
    *
    * @param roomID The ID of the room to find
    * @return RoomWrapper Pointer, The pointer to room with @param roomID in m_roomsList, if exists. Else return a new RoomWrapper object
    */
    RoomWrapper *findRoom(const QString &roomID) const;

    //Clear data and refill it with data in the cache, if there is
    void reset();
    void clear();

    void updateSubscription(const QJsonArray &array);
    void updateRoom(const QJsonObject &array);
    void addRoom(const QJsonObject &room);
    /**
    * @brief Adds a room to m_roomsList with @param room
    *
    * @param room The room to be added
    */
    void addRoom(Room *room);
protected:
    QHash<int, QByteArray> roleNames() const override;


private:
    Room *createNewRoom();
    QIcon status(Room *r) const;
    QIcon icon(Room *r) const;
    int order(Room *r) const;
    QString sectionName(Room *r) const;
    RocketChatAccount *mRocketChatAccount = nullptr;
    QVector<Room *> mRoomsList;
};

#endif // ROOMMODEL_H
