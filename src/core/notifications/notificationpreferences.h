/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QObject>

class NotificationPreferenceModel;
class NotificationDesktopDurationPreferenceModel;
class NotificationDesktopSoundPreferenceModel;
class CustomSoundsManager;
class LIBRUQOLACORE_EXPORT NotificationPreferences : public QObject
{
    Q_OBJECT
public:
    explicit NotificationPreferences(QObject *parent = nullptr);
    ~NotificationPreferences() override;

    [[nodiscard]] NotificationPreferenceModel *emailNotificationModel() const;

    [[nodiscard]] NotificationPreferenceModel *mobileNotificationModel() const;

    [[nodiscard]] NotificationPreferenceModel *desktopNotificationModel() const;

    [[nodiscard]] NotificationDesktopDurationPreferenceModel *desktopDurationNotificationModel() const;

    [[nodiscard]] NotificationDesktopSoundPreferenceModel *desktopSoundNotificationModel() const;

    [[nodiscard]] CustomSoundsManager *customSoundManager() const;
    void setCustomSoundManager(CustomSoundsManager *newCustomSoundManager);

private:
    NotificationPreferenceModel *const mEmailNotificationModel;
    NotificationPreferenceModel *const mMobileNotificationModel;
    NotificationPreferenceModel *const mDesktopNotificationModel;
    NotificationDesktopDurationPreferenceModel *const mDesktopDurationNotificationModel;
    NotificationDesktopSoundPreferenceModel *const mDesktopSoundNotificationModel;
};
