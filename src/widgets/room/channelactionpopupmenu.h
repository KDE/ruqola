/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "libruqolawidgets_private_export.h"
#include "roomheaderwidget.h"
#include <QObject>
#include <QPointer>
class QMenu;
class QAction;
class Room;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelActionPopupMenu : public QObject
{
    Q_OBJECT
public:
    explicit ChannelActionPopupMenu(QObject *parent = nullptr);
    ~ChannelActionPopupMenu() override;

    Q_REQUIRED_RESULT QMenu *menu() const;
    void setCurrentRocketChatAccount(RocketChatAccount *account);

    void setRoom(Room *room);
Q_SIGNALS:
    void actionRequested(RoomHeaderWidget::ChannelActionType type);

private:
    void slotUpdateMenu();
    void createMenu();
    QAction *createSeparator();
    QMenu *const mMenu;
    QAction *mShowMentions = nullptr;
    QAction *mShowPinnedMessages = nullptr;
    QAction *mShowStarredMessages = nullptr;
    QAction *mShowSnipperedMessages = nullptr;
    QAction *mConfigureNotification = nullptr;
    QAction *mShowFileAttachments = nullptr;
    QAction *mShowDiscussions = nullptr;
    QAction *mShowThreads = nullptr;
    QAction *mAutoTranslate = nullptr;
    QAction *mAutoTranslateSeparator = nullptr;
    QAction *mInviteUsersGenerateUrl = nullptr;
    QAction *mInviteUsersGenerateUrlSeparator = nullptr;
    QAction *mAddUserInRoomsSeparator = nullptr;
    QAction *mAddUserInRooms = nullptr;
    QAction *mStartVideoChat = nullptr;
    QAction *mPruneMessages = nullptr;
    QAction *mPruneMessagesSeparator = nullptr;
    QAction *mExportMessages = nullptr;
    QAction *mFollowMessage = nullptr;

    Room *mRoom = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};

