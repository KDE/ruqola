/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

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
class QFormLayout;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelInfoReadOnlyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelInfoReadOnlyWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ChannelInfoReadOnlyWidget() override;

    void setRoom(Room *room);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void updateReadOnlyChannelInfo();
    LIBRUQOLAWIDGETS_NO_EXPORT void connectReadOnlyWidget();
    LIBRUQOLAWIDGETS_NO_EXPORT void createCommentReadOnly();
    LIBRUQOLAWIDGETS_NO_EXPORT void createAnnouncementReadOnly();
    LIBRUQOLAWIDGETS_NO_EXPORT void createDescriptionReadOnly();
    QLabel *const mNameReadOnly;
    QLabel *mCommentReadOnly = nullptr;
    QLabel *mAnnouncementReadOnly = nullptr;
    QLabel *mDescriptionReadOnly = nullptr;
    RoomAvatarReadOnlyWidget *const mRoomAvatarWidget;
    QPointer<Room> mRoom;
    RocketChatAccount *const mRocketChatAccount;
    QFormLayout *const mLayoutReadOnly;
};
