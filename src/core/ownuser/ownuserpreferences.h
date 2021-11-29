/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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

    Q_REQUIRED_RESULT QString mobileNotifications() const;
    void setMobileNotifications(const QString &mobileNotifications);

    Q_REQUIRED_RESULT bool convertAsciiEmoji() const;
    void setConvertAsciiEmoji(bool convertAsciiEmoji);

    Q_REQUIRED_RESULT bool useEmojis() const;
    void setUseEmojis(bool useEmojis);

    Q_REQUIRED_RESULT bool hideRoles() const;
    void setHideRoles(bool hideRoles);

    Q_REQUIRED_RESULT bool hideAvatars() const;
    void setHideAvatars(bool hideAvatars);

    Q_REQUIRED_RESULT int idleTimeLimit() const;
    void setIdleTimeLimit(int newIdleTimeLimit);

    Q_REQUIRED_RESULT bool enableAutoAway() const;
    void setEnableAutoAway(bool newEnableAutoAway);

private:
    QStringList mHighlightWords;
    QString mEmailNotificationMode;
    QString mDesktopNotifications;
    QString mMobileNotifications;
    int mIdleTimeLimit = -1;
    bool mConvertAsciiEmoji = true;
    bool mUseEmojis = true;
    bool mHideRoles = false;
    bool mHideAvatars = false;
    bool mEnableAutoAway = false;
};

Q_DECLARE_METATYPE(OwnUserPreferences)
Q_DECLARE_TYPEINFO(OwnUserPreferences, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const OwnUserPreferences &t);
