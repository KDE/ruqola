/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLabel;
class Room;
class RoomAvatarWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelInfoReadOnlyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelInfoReadOnlyWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ChannelInfoReadOnlyWidget() override;

    void setRoom(Room *room);

private:
    void updateReadOnlyChannelInfo();
    void connectReadOnlyWidget();
    QLabel *const mNameReadOnly;
    QLabel *const mCommentReadOnly;
    QLabel *const mAnnouncementReadOnly;
    QLabel *const mDescriptionReadOnly;
    RoomAvatarWidget *const mRoomAvatarWidget;
    Room *mRoom = nullptr;
    RocketChatAccount *const mRocketChatAccount;
};
