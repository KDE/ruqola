/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include "libruqolacore_export.h"
#include <QObject>

class NotificationPreferenceModel;
class NotificationDesktopDurationPreferenceModel;
class NotificationDesktopSoundPreferenceModel;
class LIBRUQOLACORE_EXPORT NotificationPreferences : public QObject
{
    Q_OBJECT
public:
    explicit NotificationPreferences(QObject *parent = nullptr);
    ~NotificationPreferences() override;

    Q_REQUIRED_RESULT NotificationPreferenceModel *emailNotificationModel() const;

    Q_REQUIRED_RESULT NotificationPreferenceModel *mobileNotificationModel() const;

    Q_REQUIRED_RESULT NotificationPreferenceModel *desktopNotificationModel() const;

    Q_REQUIRED_RESULT NotificationPreferenceModel *desktopAudioNotificationModel() const;

    Q_REQUIRED_RESULT NotificationDesktopDurationPreferenceModel *desktopDurationNotificationModel() const;

    Q_REQUIRED_RESULT NotificationDesktopSoundPreferenceModel *desktopSoundNotificationModel() const;
    static NotificationPreferences *self();

private:
    NotificationPreferenceModel *const mEmailNotificationModel;
    NotificationPreferenceModel *const mMobileNotificationModel;
    NotificationPreferenceModel *const mDesktopNotificationModel;
    NotificationPreferenceModel *const mDesktopAudioNotificationModel;
    NotificationDesktopDurationPreferenceModel *const mDesktopDurationNotificationModel;
    NotificationDesktopSoundPreferenceModel *const mDesktopSoundNotificationModel;
};

