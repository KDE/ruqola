/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
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
        UserOffline,
    };
    Q_ENUM(RoomRoles)

    enum class Section {
        Unread,
        Favorites,
        Teams,
        Rooms,
        PrivateMessages,
        Discussions,
        Unknown,
        NSections,
    };

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

    void getUnreadAlertFromAccount(bool &hasAlert, int &nbUnread) const;
    void userStatusChanged(const User &user);

    UsersForRoomModel *usersModelForRoom(const QString &roomId) const;

    MessageModel *messageModel(const QString &roomId) const;

    Q_REQUIRED_RESULT QString inputMessage(const QString &roomId) const;
    void setInputMessage(const QString &roomId, const QString &inputMessage);
    Q_REQUIRED_RESULT Room *findRoom(const QString &roomID) const;
    void updateSubscriptionRoom(const QJsonObject &room);
    Q_REQUIRED_RESULT QString insertRoom(const QJsonObject &room);

    Q_REQUIRED_RESULT QModelIndex indexForRoomName(const QString &roomName) const;

    static QString sectionName(RoomModel::Section sectionId);

Q_SIGNALS:
    void needToUpdateNotification();
    void roomNeedAttention();
    void roomRemoved(const QString &roomId);

private:
    Q_DISABLE_COPY(RoomModel)
    Room *createNewRoom();
    Q_REQUIRED_RESULT bool userOffline(Room *r) const;
    Q_REQUIRED_RESULT QIcon icon(Room *r) const;
    Q_REQUIRED_RESULT int order(Room *r) const;
    Q_REQUIRED_RESULT Section section(Room *r) const;
    Q_REQUIRED_RESULT QString generateToolTip(Room *r) const;

    RocketChatAccount *const mRocketChatAccount;
    QVector<Room *> mRoomsList;
};

Q_DECLARE_METATYPE(RoomModel::Section)
