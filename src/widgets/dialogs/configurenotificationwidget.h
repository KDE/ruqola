/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QPointer>
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
    QCheckBox *const mShowBadgeMentions;
    QPointer<Room> mRoom;
    QComboBox *const mDesktopAlertCombobox;
    QComboBox *const mDesktopSoundCombobox;
    QComboBox *const mMobileAlertCombobox;
    QComboBox *const mEmailAlertCombobox;
};
