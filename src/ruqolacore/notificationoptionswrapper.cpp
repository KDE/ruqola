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

#include "notificationoptionswrapper.h"

NotificationOptionsWrapper::NotificationOptionsWrapper(QObject *parent)
    : QObject(parent)
{
}

NotificationOptionsWrapper::NotificationOptionsWrapper(const NotificationOptions &notification, QObject *parent)
    : QObject(parent)
    , mNotificationOptions(notification)
{
}

NotificationOptionsWrapper::~NotificationOptionsWrapper()
{
}

bool NotificationOptionsWrapper::hideUnreadStatus() const
{
    return mNotificationOptions.hideUnreadStatus();
}

bool NotificationOptionsWrapper::disableNotifications() const
{
    return mNotificationOptions.disableNotifications();
}

QString NotificationOptionsWrapper::unreadTrayIconAlert() const
{
    return mNotificationOptions.unreadTrayIconAlert();
}

QString NotificationOptionsWrapper::emailNotifications() const
{
    return mNotificationOptions.emailNotifications();
}

QString NotificationOptionsWrapper::mobilePushNotification() const
{
    return mNotificationOptions.mobilePushNotification();
}

QString NotificationOptionsWrapper::desktopNotifications() const
{
    return mNotificationOptions.desktopNotifications();
}

QString NotificationOptionsWrapper::audioNotifications() const
{
    return mNotificationOptions.audioNotifications();
}

QString NotificationOptionsWrapper::audioNotificationValue() const
{
    return mNotificationOptions.audioNotificationValue();
}

int NotificationOptionsWrapper::desktopNotificationDuration() const
{
    return mNotificationOptions.desktopNotificationDuration();
}

bool NotificationOptionsWrapper::muteGroupMentions() const
{
    return mNotificationOptions.muteGroupMentions();
}
