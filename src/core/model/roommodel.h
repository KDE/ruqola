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
        RoomMentionsInfoType,
    };
    Q_ENUM(RoomRoles)

    enum class Section : uint8_t {
        Unread,
        Favorites,
        Teams,
        Rooms,
        PrivateMessages,
        Discussions,
        Unknown,
        NSections,
    };

    enum class MentionsInfoType : uint8_t {
        Important,
        Warning,
        Information,
        Normal,
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
    void addRoom(const QByteArray &roomID, const QString &roomName, bool selected = false);

    // Clear data and refill it with data in the cache, if there is
    void clear();

    void updateSubscription(const QJsonArray &array);
    QByteArray updateRoom(const QJsonObject &array);
    QByteArray addRoom(const QJsonObject &room);

    [[nodiscard]] Room::TeamRoomInfo roomFromTeamId(const QByteArray &teamId);

    /**
     * @brief Adds a room to mRoomsList with @param room
     *
     * @param room The room to be added
     */
    [[nodiscard]] bool addRoom(Room *room);
    void removeRoom(const QByteArray &roomId);

    void getUnreadAlertFromAccount(bool &hasAlert, int &nbUnread, bool &hasMentions) const;
    void userStatusChanged(const User &user);

    [[nodiscard]] UsersForRoomModel *usersModelForRoom(const QByteArray &roomId) const;

    [[nodiscard]] MessagesModel *messageModel(const QByteArray &roomId) const;

    [[nodiscard]] Room *findRoom(const QByteArray &roomID) const;
    void updateSubscriptionRoom(const QJsonObject &room);
    [[nodiscard]] QByteArray insertRoom(const QJsonObject &room);

    [[nodiscard]] static QString sectionName(RoomModel::Section sectionId);

    [[nodiscard]] QList<Room *> findRoomNameConstains(const QString &str) const;
    void cleanRoomHistory();
Q_SIGNALS:
    void needToUpdateNotification();
    void roomNeedAttention();
    void roomRemoved(const QByteArray &roomId);
    void openChanged(const QByteArray &roomId);

private:
    LIBRUQOLACORE_NO_EXPORT Room *createNewRoom();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT bool userOffline(Room *r) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT Section section(Room *r) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString generateToolTip(Room *r) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString generateUnreadToolTip(Room *r) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT RoomModel::MentionsInfoType mentionsInfoType(Room *r) const;

    RocketChatAccount *const mRocketChatAccount;
    QList<Room *> mRoomsList;
};

Q_DECLARE_METATYPE(RoomModel::Section)
