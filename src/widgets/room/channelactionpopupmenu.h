/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

    [[nodiscard]] QMenu *menu() const;
    void setCurrentRocketChatAccount(RocketChatAccount *account);

    void setRoom(Room *room);
Q_SIGNALS:
    void actionRequested(RoomHeaderWidget::ChannelActionType type);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateMenu();
    LIBRUQOLAWIDGETS_NO_EXPORT void createMenu();
    LIBRUQOLAWIDGETS_NO_EXPORT QAction *createSeparator();
    QMenu *const mMenu;
    QAction *mShowMentions = nullptr;
    QAction *mShowPinnedMessages = nullptr;
    QAction *mShowStarredMessages = nullptr;
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
    QAction *mOffTheRecordMessages = nullptr;
    QAction *mEncryptMessages = nullptr;

    QPointer<Room> mRoom;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
