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

#pragma once
#include "libruqolacore_export.h"
#include "room.h"
#include "user.h"
#include <QAbstractListModel>
#include <QObject>
class RocketChatAccount;
class MessageModel;

class LIBRUQOLACORE_EXPORT RoomModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum RoomRoles {
        RoomName = Qt::UserRole + 1,
        RoomFName,
        RoomSelected,
        RoomId,
        RoomUnread,
        RoomType,
        RoomOwnerUserName, // created by UserName
        RoomOwnerUserId,
        RoomTopic,
        RoomMutedUsers,
        RoomJitsiTimeout,
        RoomReadOnly,
        RoomAnnouncement,
        RoomOpen,
        RoomAlert,
        RoomOrder,
        RoomFavorite,
        RoomSection,
        RoomIcon,
        RoomOtr,
        RoomUserMentions,
        RoomIgnoredUsers,
        RoomAutotranslateLanguage,
        RoomAutotranslate,
        RoomDirectChannelUserId,
        RoomAvatarInfo,
        RoomTeamId,
        RoomTeamIsMain,
        RoomTeamName,
    };
    Q_ENUM(RoomRoles)

    explicit RoomModel(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    ~RoomModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Constructs room object from @param roomID and @param roomName and @param selected, then calls @method addRoom
     *
     * @param roomID The unique room ID
     * @param roomName The name of the room
     * @param selected True if room if @param roomID is selected, else false
     */
    void addRoom(const QString &roomID, const QString &roomName, bool selected = false);

    // Clear data and refill it with data in the cache, if there is
    void reset();
    void clear();

    void updateSubscription(const QJsonArray &array);
    void updateRoom(const QJsonObject &array);
    Room *addRoom(const QJsonObject &room);

    Q_REQUIRED_RESULT Room::TeamRoomInfo roomFromTeamId(const QString &teamId);

    /**
     * @brief Adds a room to mRoomsList with @param room
     *
     * @param room The room to be added
     */
    Q_REQUIRED_RESULT bool addRoom(Room *room);
    void removeRoom(const QString &roomId);

    void getUnreadAlertFromAccount(bool &hasAlert, int &nbUnread);
    void userStatusChanged(const User &user);

    UsersForRoomModel *usersModelForRoom(const QString &roomId) const;

    MessageModel *messageModel(const QString &roomId) const;

    Q_REQUIRED_RESULT QString inputMessage(const QString &roomId) const;
    void setInputMessage(const QString &roomId, const QString &inputMessage);
    Q_REQUIRED_RESULT Room *findRoom(const QString &roomID) const;
    void updateSubscriptionRoom(const QJsonObject &room);
    Q_REQUIRED_RESULT QString insertRoom(const QJsonObject &room);

    Q_REQUIRED_RESULT QModelIndex indexForRoomName(const QString &roomName) const;
Q_SIGNALS:
    void needToUpdateNotification();
    void roomNeedAttention();

private:
    Q_DISABLE_COPY(RoomModel)
    Room *createNewRoom();
    Q_REQUIRED_RESULT QString roomTeamName(Room *r) const;
    Q_REQUIRED_RESULT QIcon icon(Room *r) const;
    Q_REQUIRED_RESULT int order(Room *r) const;
    Q_REQUIRED_RESULT QString sectionName(Room *r) const;

    RocketChatAccount *const mRocketChatAccount;
    QVector<Room *> mRoomsList;
};

