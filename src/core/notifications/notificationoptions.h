/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QString>

class LIBRUQOLACORE_EXPORT NotificationOptions
{
    Q_GADGET
public:
    NotificationOptions();
    NotificationOptions(const NotificationOptions &other) = default;
    struct LIBRUQOLACORE_EXPORT NotificationValue {
        NotificationValue() = default;
        explicit NotificationValue(const QString &val, const QString pref)
            : value(val)
            , preferenceOrigin(pref)
        {
        }
        QString value;
        QString preferenceOrigin;
        [[nodiscard]] bool operator==(const NotificationValue &other) const;
        [[nodiscard]] QString currentValue() const;
        [[nodiscard]] bool isEmpty() const;
    };

    enum NotificationType {
        Default = 0,
        AllMessages,
        Mentions,
        Nothing,
    };
    Q_ENUM(NotificationType)

    [[nodiscard]] bool hideUnreadStatus() const;
    void setHideUnreadStatus(bool value);

    [[nodiscard]] bool disableNotifications() const;
    void setDisableNotifications(bool disableNotifications);

    [[nodiscard]] QString unreadTrayIconAlert() const;
    void setUnreadTrayIconAlert(const QString &unreadTrayIconAlert);

    [[nodiscard]] NotificationValue emailNotifications() const;
    void setEmailNotifications(const NotificationValue &emailNotifications);

    [[nodiscard]] NotificationValue mobilePushNotification() const;
    void setMobilePushNotification(const NotificationValue &mobilePushNotification);

    [[nodiscard]] NotificationValue desktopNotifications() const;
    void setDesktopNotifications(const NotificationValue &desktopNotifications);

    [[nodiscard]] QString audioNotificationValue() const;
    void setAudioNotificationValue(const QString &audioNotificationValue);

    [[nodiscard]] static QJsonObject serialize(const NotificationOptions &message);
    [[nodiscard]] static NotificationOptions deserialize(const QJsonObject &o);

    void parseNotificationOptions(const QJsonObject &obj);
    void updateNotificationOptions(const QJsonObject &obj);

    NotificationOptions &operator=(const NotificationOptions &other) = default;
    [[nodiscard]] bool operator==(const NotificationOptions &other) const;
    [[nodiscard]] bool operator!=(const NotificationOptions &other) const;

    [[nodiscard]] bool muteGroupMentions() const;
    void setMuteGroupMentions(bool muteGroupMentions);

    [[nodiscard]] bool hideMentionStatus() const;
    void setHideMentionStatus(bool newMhideMentionStatus);

private:
    NotificationValue mDesktopNotifications;
    NotificationValue mMobilePushNotification;
    NotificationValue mEmailNotifications;

    // TODO use enums ????
    QString mUnreadTrayIconAlert;
    QString mAudioNotificationValue; // Type
    bool mDisableNotifications = false;
    bool mHideUnreadStatus = false;
    bool mMuteGroupMentions = false;
    bool mHideMentionStatus = false;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const NotificationOptions &t);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const NotificationOptions::NotificationValue &t);
