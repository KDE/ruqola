/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "notificationpreferences.h"
#include "model/notificationpreferencemodel.h"
#include "model/notificationdesktopdurationpreferencemodel.h"

NotificationPreferences::NotificationPreferences(QObject *parent)
    : QObject(parent)
{
    mEmailNotificationModel = new NotificationPreferenceModel(this);
    mMobileNotificationModel = new NotificationPreferenceModel(this);
    mDesktopNotificationModel = new NotificationPreferenceModel(this);
    mDesktopAudioNotificationModel = new NotificationPreferenceModel(this);
    mDesktopDurationNotificationModel = new NotificationDesktopDurationPreferenceModel(this);
}

NotificationPreferences::~NotificationPreferences()
{
}

NotificationPreferenceModel *NotificationPreferences::emailNotificationModel() const
{
    return mEmailNotificationModel;
}

NotificationPreferenceModel *NotificationPreferences::mobileNotificationModel() const
{
    return mMobileNotificationModel;
}

NotificationPreferenceModel *NotificationPreferences::desktopNotificationModel() const
{
    return mDesktopNotificationModel;
}

NotificationPreferenceModel *NotificationPreferences::desktopAudioNotificationModel() const
{
    return mDesktopAudioNotificationModel;
}

NotificationDesktopDurationPreferenceModel *NotificationPreferences::desktopDurationNotificationModel() const
{
    return mDesktopDurationNotificationModel;
}
