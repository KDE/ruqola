/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"

#include <QDebug>

class LIBRUQOLACORE_EXPORT OwnUserPreferences
{
    Q_GADGET
public:
    OwnUserPreferences();
    ~OwnUserPreferences();
    void parsePreferences(const QJsonObject &replyObject);
    [[nodiscard]] bool operator==(const OwnUserPreferences &other) const;
    [[nodiscard]] QStringList highlightWords() const;
    void setHighlightWords(const QStringList &highlightWords);

    [[nodiscard]] QString emailNotificationMode() const;
    void setEmailNotificationMode(const QString &emailNotificationMode);

    [[nodiscard]] QString desktopNotifications() const;
    void setDesktopNotifications(const QString &desktopNotifications);

    [[nodiscard]] bool convertAsciiEmoji() const;
    void setConvertAsciiEmoji(bool convertAsciiEmoji);

    [[nodiscard]] bool useEmojis() const;
    void setUseEmojis(bool useEmojis);

    [[nodiscard]] bool hideRoles() const;
    void setHideRoles(bool hideRoles);

    [[nodiscard]] bool displayAvatars() const;
    void setDisplayAvatars(bool hideAvatars);

    [[nodiscard]] int idleTimeLimit() const;
    void setIdleTimeLimit(int newIdleTimeLimit);

    [[nodiscard]] bool enableAutoAway() const;
    void setEnableAutoAway(bool newEnableAutoAway);

    void updateHighlightWords(const QJsonArray &highlightsArray);

    void setPushNotifications(const QString &pushNotifications);
    [[nodiscard]] QString pushNotifications() const;

    [[nodiscard]] bool showUnread() const;
    void setShowUnread(bool newShowUnread);

    enum class RoomListSortOrder : uint8_t {
        Unknown,
        ByLastMessage,
        Alphabetically,
    };
    Q_ENUM(RoomListSortOrder)
    [[nodiscard]] RoomListSortOrder roomListSortOrder() const;
    void setRoomListSortOrder(RoomListSortOrder roomListSortOrder);

    enum class RoomListDisplay : uint8_t {
        Unknown,
        Condensed,
        Medium,
        Extended,
    };
    Q_ENUM(RoomListDisplay)

    [[nodiscard]] RoomListDisplay roomListDisplay() const;
    void setRoomListDisplay(RoomListDisplay newRoomListDisplay);

    [[nodiscard]] bool showRoomAvatar() const;
    void setShowRoomAvatar(bool newShowRoomAvatar);

    [[nodiscard]] bool showFavorite() const;
    void setShowFavorite(bool newShowFavorite);

    [[nodiscard]] bool receiveLoginDetectionEmail() const;
    void setReceiveLoginDetectionEmail(bool newReceiveLoginDetectionEmail);

    [[nodiscard]] QByteArray newMessageNotification() const;
    void setNewMessageNotification(const QByteArray &newNewMessageNotification);

    [[nodiscard]] QByteArray newRoomNotification() const;
    void setNewRoomNotification(const QByteArray &newNewRoomNotification);

    [[nodiscard]] int notificationsSoundVolume() const;
    void setNotificationsSoundVolume(int newNotificationsSoundVolume);

    [[nodiscard]] bool muteFocusedConversations() const;
    void setMuteFocusedConversations(bool newMuteFocusedConversations);

private:
    QStringList mHighlightWords;
    QString mEmailNotificationMode;
    QString mDesktopNotifications;
    QString mPushNotifications;
    QByteArray mNewMessageNotification;
    QByteArray mNewRoomNotification;
    RoomListSortOrder mRoomListSortOrder = RoomListSortOrder::Unknown;
    RoomListDisplay mRoomListDisplay = RoomListDisplay::Unknown;
    int mIdleTimeLimit = -1;
    int mNotificationsSoundVolume = -1;
    bool mConvertAsciiEmoji = true;
    bool mUseEmojis = true;
    bool mHideRoles = false;
    bool mDisplayAvatars = true;
    bool mEnableAutoAway = false;
    bool mShowUnread = false;
    bool mShowRoomAvatar = false;
    bool mShowFavorite = true;
    bool mReceiveLoginDetectionEmail = false;
    bool mMuteFocusedConversations = false;
};
Q_DECLARE_TYPEINFO(OwnUserPreferences, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const OwnUserPreferences &t);
