/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

    Q_REQUIRED_RESULT NotificationDesktopDurationPreferenceModel *desktopDurationNotificationModel() const;

    Q_REQUIRED_RESULT NotificationDesktopSoundPreferenceModel *desktopSoundNotificationModel() const;
    static NotificationPreferences *self();

private:
    NotificationPreferenceModel *const mEmailNotificationModel;
    NotificationPreferenceModel *const mMobileNotificationModel;
    NotificationPreferenceModel *const mDesktopNotificationModel;
    NotificationDesktopDurationPreferenceModel *const mDesktopDurationNotificationModel;
    NotificationDesktopSoundPreferenceModel *const mDesktopSoundNotificationModel;
};
