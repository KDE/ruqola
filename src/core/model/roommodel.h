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
class QObject;
class RocketChatAccount;
class MessagesModel;

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
        RoomLastMessageAt,
        UserOffline,
        HideBadgeForMention,
        RoomGroupMentions,
        RoomThreadUnread,
        RoomUnreadToolTip,
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

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

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
    void addRoom(const QJsonObject &room);

    [[nodiscard]] Room::TeamRoomInfo roomFromTeamId(const QString &teamId);

    /**
     * @brief Adds a room to mRoomsList with @param room
     *
     * @param room The room to be added
     */
    [[nodiscard]] bool addRoom(Room *room);
    void removeRoom(const QString &roomId);

    void getUnreadAlertFromAccount(bool &hasAlert, int &nbUnread) const;
    void userStatusChanged(const User &user);

    UsersForRoomModel *usersModelForRoom(const QString &roomId) const;

    MessagesModel *messageModel(const QString &roomId) const;

    [[nodiscard]] QString inputMessage(const QString &roomId) const;
    void setInputMessage(const QString &roomId, const QString &inputMessage);
    [[nodiscard]] Room *findRoom(const QString &roomID) const;
    void updateSubscriptionRoom(const QJsonObject &room);
    [[nodiscard]] QString insertRoom(const QJsonObject &room);

    [[nodiscard]] QModelIndex indexForRoomName(const QString &roomName) const;

    static QString sectionName(RoomModel::Section sectionId);

    [[nodiscard]] QVector<Room *> findRoomNameConstains(const QString &str) const;
Q_SIGNALS:
    void needToUpdateNotification();
    void roomNeedAttention();
    void roomRemoved(const QString &roomId);

private:
    LIBRUQOLACORE_NO_EXPORT Room *createNewRoom();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT bool userOffline(Room *r) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QIcon icon(Room *r) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT Section section(Room *r) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString generateToolTip(Room *r) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString generateUnreadToolTip(Room *r) const;

    RocketChatAccount *const mRocketChatAccount;
    QVector<Room *> mRoomsList;
};

Q_DECLARE_METATYPE(RoomModel::Section)
