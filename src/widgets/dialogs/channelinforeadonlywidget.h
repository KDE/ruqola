/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QPointer>
#include <QWidget>
class QLabel;
class Room;
class RoomAvatarReadOnlyWidget;
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
    RoomAvatarReadOnlyWidget *const mRoomAvatarWidget;
    QPointer<Room> mRoom;
    RocketChatAccount *const mRocketChatAccount;
};
