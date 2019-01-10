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

#ifndef NOTIFICATIONOPTIONSWRAPPER_H
#define NOTIFICATIONOPTIONSWRAPPER_H

#include <QObject>
#include "notificationoptions.h"

class NotificationOptionsWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hideUnreadStatus READ hideUnreadStatus CONSTANT)
    Q_PROPERTY(bool disableNotifications READ disableNotifications CONSTANT)
    Q_PROPERTY(QString unreadTrayIconAlert READ unreadTrayIconAlert CONSTANT)
    Q_PROPERTY(QString emailNotifications READ emailNotifications CONSTANT)
    Q_PROPERTY(QString mobilePushNotification READ mobilePushNotification CONSTANT)
    Q_PROPERTY(QString desktopNotifications READ desktopNotifications CONSTANT)
    Q_PROPERTY(QString audioNotifications READ audioNotifications CONSTANT)
    Q_PROPERTY(QString audioNotificationValue READ audioNotificationValue CONSTANT)
    Q_PROPERTY(int desktopNotificationDuration READ desktopNotificationDuration CONSTANT)
    Q_PROPERTY(bool muteGroupMentions READ muteGroupMentions CONSTANT)

public:
    explicit NotificationOptionsWrapper(QObject *parent = nullptr);
    explicit NotificationOptionsWrapper(const NotificationOptions &notification, QObject *parent = nullptr);
    ~NotificationOptionsWrapper();

    Q_REQUIRED_RESULT bool hideUnreadStatus() const;

    Q_REQUIRED_RESULT bool disableNotifications() const;

    Q_REQUIRED_RESULT QString unreadTrayIconAlert() const;

    Q_REQUIRED_RESULT QString emailNotifications() const;

    Q_REQUIRED_RESULT QString mobilePushNotification() const;

    Q_REQUIRED_RESULT QString desktopNotifications() const;

    Q_REQUIRED_RESULT QString audioNotifications() const;

    Q_REQUIRED_RESULT QString audioNotificationValue() const;

    Q_REQUIRED_RESULT int desktopNotificationDuration() const;

    Q_REQUIRED_RESULT bool muteGroupMentions() const;

private:
    Q_DISABLE_COPY(NotificationOptionsWrapper)
    NotificationOptions mNotificationOptions;
};

#endif // NOTIFICATIONOPTIONSWRAPPER_H
