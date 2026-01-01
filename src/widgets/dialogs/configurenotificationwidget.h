/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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
class SoundConfigureWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureNotificationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureNotificationWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ConfigureNotificationWidget() override;
    [[nodiscard]] Room *room() const;
    void setRoom(Room *room);

private:
    QCheckBox *const mDisableNotification;
    QCheckBox *const mHideUnreadRoomStatus;
    QCheckBox *const mMuteGroupMentions;
    QCheckBox *const mShowBadgeMentions;
    QPointer<Room> mRoom;
    QComboBox *const mDesktopAlertCombobox;
    SoundConfigureWidget *const mDesktopSoundConfigureWidget;
    QComboBox *const mMobileAlertCombobox;
    QComboBox *const mEmailAlertCombobox;
    RocketChatAccount *const mRocketChatAccount;
};
