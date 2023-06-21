/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationpreferences.h"
#include "model/notificationdesktopdurationpreferencemodel.h"
#include "model/notificationdesktopsoundpreferencemodel.h"
#include "model/notificationpreferencemodel.h"

NotificationPreferences::NotificationPreferences(QObject *parent)
    : QObject(parent)
    , mEmailNotificationModel(new NotificationPreferenceModel(this))
    , mMobileNotificationModel(new NotificationPreferenceModel(this))
    , mDesktopNotificationModel(new NotificationPreferenceModel(this))
    , mDesktopDurationNotificationModel(new NotificationDesktopDurationPreferenceModel(this))
    , mDesktopSoundNotificationModel(new NotificationDesktopSoundPreferenceModel(this))
{
}

NotificationPreferences::~NotificationPreferences() = default;

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

NotificationDesktopDurationPreferenceModel *NotificationPreferences::desktopDurationNotificationModel() const
{
    return mDesktopDurationNotificationModel;
}

NotificationDesktopSoundPreferenceModel *NotificationPreferences::desktopSoundNotificationModel() const
{
    return mDesktopSoundNotificationModel;
}

NotificationPreferences *NotificationPreferences::NotificationPreferences::self()
{
    static NotificationPreferences s_self;
    return &s_self;
}

#include "moc_notificationpreferences.cpp"
