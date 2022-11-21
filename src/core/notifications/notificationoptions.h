/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_EXPORT NotificationOptions
{
    Q_GADGET
public:
    NotificationOptions();
    NotificationOptions(const NotificationOptions &other) = default;
    struct LIBRUQOLACORE_EXPORT NotificationValues {
        NotificationValues() = default;
        explicit NotificationValues(const QString &val, const QString pref)
            : value(val)
            , preferenceOrigin(pref)
        {
        }
        QString value;
        QString preferenceOrigin;
        Q_REQUIRED_RESULT bool operator==(const NotificationValues &other) const;
        Q_REQUIRED_RESULT QString currentValue() const;
        Q_REQUIRED_RESULT bool isEmpty() const;
    };

    enum NotificationType {
        Default = 0,
        AllMessages,
        Mentions,
        Nothing,
    };
    Q_ENUM(NotificationType)

    Q_REQUIRED_RESULT bool hideUnreadStatus() const;
    void setHideUnreadStatus(bool value);

    Q_REQUIRED_RESULT bool disableNotifications() const;
    void setDisableNotifications(bool disableNotifications);

    Q_REQUIRED_RESULT QString unreadTrayIconAlert() const;
    void setUnreadTrayIconAlert(const QString &unreadTrayIconAlert);

    Q_REQUIRED_RESULT NotificationValues emailNotifications() const;
    void setEmailNotifications(const NotificationValues &emailNotifications);

    Q_REQUIRED_RESULT NotificationValues mobilePushNotification() const;
    void setMobilePushNotification(const NotificationValues &mobilePushNotification);

    Q_REQUIRED_RESULT NotificationValues desktopNotifications() const;
    void setDesktopNotifications(const NotificationValues &desktopNotifications);

    Q_REQUIRED_RESULT QString audioNotificationValue() const;
    void setAudioNotificationValue(const QString &audioNotificationValue);

    Q_REQUIRED_RESULT static QJsonObject serialize(const NotificationOptions &message);
    Q_REQUIRED_RESULT static NotificationOptions fromJSon(const QJsonObject &o);

    void parseNotificationOptions(const QJsonObject &obj);
    void updateNotificationOptions(const QJsonObject &obj);

    NotificationOptions &operator=(const NotificationOptions &other) = default;
    Q_REQUIRED_RESULT bool operator==(const NotificationOptions &other) const;
    Q_REQUIRED_RESULT bool operator!=(const NotificationOptions &other) const;

    Q_REQUIRED_RESULT bool muteGroupMentions() const;
    void setMuteGroupMentions(bool muteGroupMentions);

private:
    NotificationValues mDesktopNotifications;
    NotificationValues mMobilePushNotification;
    NotificationValues mEmailNotifications;

    // TODO use enums ????
    QString mUnreadTrayIconAlert;
    QString mAudioNotificationValue; // Type
    bool mDisableNotifications = false;
    bool mHideUnreadStatus = false;
    bool mMuteGroupMentions = false;
    bool mhideMentionStatus = false; // TODO ?
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const NotificationOptions &t);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const NotificationOptions::NotificationValues &t);
