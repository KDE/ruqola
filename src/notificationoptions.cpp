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

#include "notificationoptions.h"

NotificationOptions::NotificationOptions()
{

}

bool NotificationOptions::hideUnreadStatus() const
{
    return mHideUnreadStatus;
}

void NotificationOptions::setHideUnreadStatus(bool value)
{
    mHideUnreadStatus = value;
}

bool NotificationOptions::disableNotifications() const
{
    return mDisableNotifications;
}

void NotificationOptions::setDisableNotifications(bool disableNotifications)
{
    mDisableNotifications = disableNotifications;
}

QString NotificationOptions::unreadTrayIconAlert() const
{
    return mUnreadTrayIconAlert;
}

void NotificationOptions::setUnreadTrayIconAlert(const QString &unreadTrayIconAlert)
{
    mUnreadTrayIconAlert = unreadTrayIconAlert;
}

QString NotificationOptions::emailNotifications() const
{
    return mEmailNotifications;
}

void NotificationOptions::setEmailNotifications(const QString &emailNotifications)
{
    mEmailNotifications = emailNotifications;
}

QString NotificationOptions::mobilePushNotification() const
{
    return mMobilePushNotification;
}

void NotificationOptions::setMobilePushNotification(const QString &mobilePushNotification)
{
    mMobilePushNotification = mobilePushNotification;
}

QString NotificationOptions::desktopNotifications() const
{
    return mDesktopNotifications;
}

void NotificationOptions::setDesktopNotifications(const QString &desktopNotifications)
{
    mDesktopNotifications = desktopNotifications;
}

QString NotificationOptions::audioNotifications() const
{
    return mAudioNotifications;
}

void NotificationOptions::setAudioNotifications(const QString &audioNotifications)
{
    mAudioNotifications = audioNotifications;
}

bool NotificationOptions::operator==(const NotificationOptions &other) const
{
    return (mAudioNotifications == other.audioNotifications())
            && (mDesktopNotifications == other.desktopNotifications())
            && (mMobilePushNotification == other.mobilePushNotification())
            && (mEmailNotifications == other.emailNotifications())
            && (mUnreadTrayIconAlert == other.unreadTrayIconAlert())
            && (mDisableNotifications == other.disableNotifications())
            && (mHideUnreadStatus == other.hideUnreadStatus());
}

QDebug operator <<(QDebug d, const NotificationOptions &t)
{
    d << "mAudioNotifications: " << t.audioNotifications();
    d << "mDesktopNotifications: " << t.desktopNotifications();
    d << "mMobilePushNotification: " << t.mobilePushNotification();
    d << "mEmailNotifications: " << t.emailNotifications();
    d << "mDisableNotifications: " << t.disableNotifications();
    d << "hideUnreadStatus: " << t.hideUnreadStatus();
    return d;
}
