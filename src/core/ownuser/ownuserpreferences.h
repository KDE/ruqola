/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"

#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT OwnUserPreferences
{
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

    Q_REQUIRED_RESULT bool showRoomAvatar() const;
    void setShowRoomAvatar(bool newShowRoomAvatar);

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
    bool mShowRoomAvatar = false;
};

Q_DECLARE_METATYPE(OwnUserPreferences)
Q_DECLARE_TYPEINFO(OwnUserPreferences, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const OwnUserPreferences &t);
