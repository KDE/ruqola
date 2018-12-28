/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef NOTIFICATIONOPTIONS_H
#define NOTIFICATIONOPTIONS_H

#include <QString>
#include <QDebug>
#include <QJsonObject>
#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT NotificationOptions
{
    Q_GADGET
public:
    NotificationOptions();

    enum NotificationType {
        Default = 0,
        AllMessages,
        Mentions,
        Nothing
    };
    Q_ENUM(NotificationType)

    Q_REQUIRED_RESULT bool hideUnreadStatus() const;
    void setHideUnreadStatus(bool value);

    Q_REQUIRED_RESULT bool disableNotifications() const;
    void setDisableNotifications(bool disableNotifications);

    Q_REQUIRED_RESULT QString unreadTrayIconAlert() const;
    void setUnreadTrayIconAlert(const QString &unreadTrayIconAlert);

    Q_REQUIRED_RESULT QString emailNotifications() const;
    void setEmailNotifications(const QString &emailNotifications);

    Q_REQUIRED_RESULT QString mobilePushNotification() const;
    void setMobilePushNotification(const QString &mobilePushNotification);

    Q_REQUIRED_RESULT QString desktopNotifications() const;
    void setDesktopNotifications(const QString &desktopNotifications);

    Q_REQUIRED_RESULT QString audioNotifications() const;
    void setAudioNotifications(const QString &audioNotifications);


    Q_REQUIRED_RESULT QString audioNotificationValue() const;
    void setAudioNotificationValue(const QString &audioNotificationValue);

    Q_REQUIRED_RESULT int desktopNotificationDuration() const;
    void setDesktopNotificationDuration(int desktopNotificationDuration);

    Q_REQUIRED_RESULT static QJsonObject serialize(const NotificationOptions &message);
    Q_REQUIRED_RESULT static NotificationOptions fromJSon(const QJsonObject &o);

    void parseNotificationOptions(const QJsonObject &obj);
    void updateNotificationOptions(const QJsonObject &obj);

    NotificationOptions &operator =(const NotificationOptions &other);
    Q_REQUIRED_RESULT bool operator==(const NotificationOptions &other) const;
    //TODO add signal
private:
    //TODO use enums ????
    QString mAudioNotifications;
    QString mDesktopNotifications;
    QString mMobilePushNotification;
    QString mEmailNotifications;
    QString mUnreadTrayIconAlert;
    QString mAudioNotificationValue; //Type
    int mDesktopNotificationDuration = 0; //seconds
    bool mDisableNotifications = false;
    bool mHideUnreadStatus = false;
};
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const NotificationOptions &t);

#endif // NOTIFICATIONOPTIONS_H
