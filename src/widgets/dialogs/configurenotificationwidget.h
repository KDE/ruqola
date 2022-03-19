/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QCheckBox;
class Room;
class QComboBox;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureNotificationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureNotificationWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ConfigureNotificationWidget() override;
    Q_REQUIRED_RESULT Room *room() const;
    void setRoom(Room *room);

private:
    QCheckBox *const mDisableNotification;
    QCheckBox *const mHideUnreadRoomStatus;
    QCheckBox *const mMuteGroupMentions;
    Room *mRoom = nullptr;
    QComboBox *const mDesktopAlertCombobox;
    QComboBox *const mDesktopAudioCombobox;
    QComboBox *const mDesktopSoundCombobox;
    QComboBox *const mMobileAlertCombobox;
    QComboBox *const mEmailAlertCombobox;
};
