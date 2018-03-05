/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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
#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT NotificationOptions
{
    Q_GADGET
public:
    NotificationOptions();

    bool hideUnreadStatus() const;
    void setHideUnreadStatus(bool value);

    bool disableNotifications() const;
    void setDisableNotifications(bool disableNotifications);

    QString unreadTrayIconAlert() const;
    void setUnreadTrayIconAlert(const QString &unreadTrayIconAlert);

    QString emailNotifications() const;
    void setEmailNotifications(const QString &emailNotifications);

    QString mobilePushNotification() const;
    void setMobilePushNotification(const QString &mobilePushNotification);

    QString desktopNotifications() const;
    void setDesktopNotifications(const QString &desktopNotifications);

    QString audioNotifications() const;
    void setAudioNotifications(const QString &audioNotifications);

    bool operator==(const NotificationOptions &other) const;

    void parseNotificationOptions(const QJsonObject &obj);

    QString audioNotificationValue() const;
    void setAudioNotificationValue(const QString &audioNotificationValue);

    int desktopNotificationDuration() const;
    void setDesktopNotificationDuration(int desktopNotificationDuration);

    static QJsonObject serialize(const NotificationOptions &message);
    static NotificationOptions fromJSon(const QJsonObject &o);

    void updateNotificationOptions(const QJsonObject &obj);
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
