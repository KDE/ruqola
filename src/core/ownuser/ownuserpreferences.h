/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"

#include <QDebug>
#include <QJsonObject>
#include <QObject>

class LIBRUQOLACORE_EXPORT OwnUserPreferences
{
    Q_GADGET
public:
    OwnUserPreferences();
    ~OwnUserPreferences();
    void parsePreferences(const QJsonObject &replyObject);
    Q_REQUIRED_RESULT bool operator==(const OwnUserPreferences &other) const;
    Q_REQUIRED_RESULT QStringList highlightWords() const;
    void setHighlightWords(const QStringList &highlightWords);

    Q_REQUIRED_RESULT QString emailNotificationMode() const;
    void setEmailNotificationMode(const QString &emailNotificationMode);

    Q_REQUIRED_RESULT QString desktopNotifications() const;
    void setDesktopNotifications(const QString &desktopNotifications);

    Q_REQUIRED_RESULT bool convertAsciiEmoji() const;
    void setConvertAsciiEmoji(bool convertAsciiEmoji);

    Q_REQUIRED_RESULT bool useEmojis() const;
    void setUseEmojis(bool useEmojis);

    Q_REQUIRED_RESULT bool hideRoles() const;
    void setHideRoles(bool hideRoles);

    Q_REQUIRED_RESULT bool displayAvatars() const;
    void setDisplayAvatars(bool hideAvatars);

    Q_REQUIRED_RESULT int idleTimeLimit() const;
    void setIdleTimeLimit(int newIdleTimeLimit);

    Q_REQUIRED_RESULT bool enableAutoAway() const;
    void setEnableAutoAway(bool newEnableAutoAway);

    void updateHighlightWords(const QJsonArray &highlightsArray);

    void setPushNotifications(const QString &pushNotifications);
    Q_REQUIRED_RESULT QString pushNotifications() const;

    Q_REQUIRED_RESULT int messageViewMode() const;
    void setMessageViewMode(int newMessageViewMode);

    Q_REQUIRED_RESULT bool showUnread() const;
    void setShowUnread(bool newShowUnread);

    enum class RoomListSortOrder {
        Unknown,
        ByLastMessage,
        Alphabetically,
    };
    Q_ENUM(RoomListSortOrder)
    Q_REQUIRED_RESULT RoomListSortOrder roomListSortOrder() const;
    void setRoomListSortOrder(RoomListSortOrder roomListSortOrder);

    Q_REQUIRED_RESULT bool showRoomAvatar() const;
    void setShowRoomAvatar(bool newShowRoomAvatar);

    Q_REQUIRED_RESULT bool showFavorite() const;
    void setShowFavorite(bool newShowFavorite);

    Q_REQUIRED_RESULT bool receiveLoginDetectionEmail() const;
    void setReceiveLoginDetectionEmail(bool newReceiveLoginDetectionEmail);

private:
    QStringList mHighlightWords;
    QString mEmailNotificationMode;
    QString mDesktopNotifications;
    QString mPushNotifications;
    int mIdleTimeLimit = -1;
    int mMessageViewMode = -1;
    bool mConvertAsciiEmoji = true;
    bool mUseEmojis = true;
    bool mHideRoles = false;
    bool mDisplayAvatars = true;
    bool mEnableAutoAway = false;
    bool mShowUnread = false;
    RoomListSortOrder mRoomListSortOrder = RoomListSortOrder::Unknown;
    bool mShowRoomAvatar = false;
    bool mShowFavorite = true;
    bool mReceiveLoginDetectionEmail = false;
};
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(OwnUserPreferences)
#endif
Q_DECLARE_TYPEINFO(OwnUserPreferences, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const OwnUserPreferences &t);
