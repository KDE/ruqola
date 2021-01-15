/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "usersinroommenu.h"
#include "roomutil.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "dialogs/directchannelinfodialog.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QAction>
#include <QMenu>

UsersInRoomMenu::UsersInRoomMenu(QObject *parent)
    : QObject(parent)
{
}

UsersInRoomMenu::~UsersInRoomMenu()
{
}

void UsersInRoomMenu::slotOpenConversation()
{
    Q_EMIT Ruqola::self()->rocketChatAccount()->openLinkRequested(RoomUtil::generateUserLink(mUserName));
}

void UsersInRoomMenu::slotBlockUser()
{
    const bool userIsBlocked = mRoom->blocker();
    if (!userIsBlocked) {
        if (KMessageBox::No == KMessageBox::questionYesNo(mParentWidget, i18n("Do you want to block this user?"), i18n("Block User"))) {
            return;
        }
    }
    Ruqola::self()->rocketChatAccount()->blockUser(mRoom->roomId(), !userIsBlocked);
}

void UsersInRoomMenu::slotIgnoreUser()
{
    const bool userIsIgnored = mRoom->userIsIgnored(mUserId);
    if (!userIsIgnored) {
        if (KMessageBox::No == KMessageBox::questionYesNo(mParentWidget, i18n("Do you want to ignore this user?"), i18n("Ignore User"))) {
            return;
        }
    }
    Ruqola::self()->rocketChatAccount()->ignoreUser(mRoom->roomId(), mUserId, !userIsIgnored);
}

void UsersInRoomMenu::slotRemoveFromRoom()
{
    Ruqola::self()->rocketChatAccount()->kickUser(mRoom->roomId(), mUserId, mRoom->channelType());
}

void UsersInRoomMenu::slotCustomContextMenuRequested(const QPoint &pos)
{
    const bool canManageUsersInRoom = mRoom->canChangeRoles();
    const QString ownUserId = Ruqola::self()->rocketChatAccount()->userId();
    const bool isAdirectChannel = mRoom->channelType() == QStringLiteral("d");
    const bool isNotMe = mUserId != ownUserId;
    QMenu menu(mParentWidget);

    if (isNotMe && !isAdirectChannel) {
        auto *conversationAction = new QAction(i18n("Start Conversation"), &menu);
        connect(conversationAction, &QAction::triggered, this, &UsersInRoomMenu::slotOpenConversation);
        menu.addAction(conversationAction);
    }
    if (isNotMe) {
        if (!menu.isEmpty()) {
            menu.addSeparator();
        }
        auto *userInfoAction = new QAction(i18n("User Info"), &menu);
        userInfoAction->setIcon(QIcon::fromTheme(QStringLiteral("documentinfo")));
        connect(userInfoAction, &QAction::triggered, this, &UsersInRoomMenu::slotUserInfo);
        menu.addAction(userInfoAction);
    }
    if (canManageUsersInRoom && !isAdirectChannel) {
        if (!menu.isEmpty()) {
            menu.addSeparator();
        }
        const bool hasOwnerRole = mRoom->userHasOwnerRole(mUserId);
        auto *removeAsUser = new QAction(hasOwnerRole ? i18n("Remove as Owner") : i18n("Add as Owner"), &menu);
        connect(removeAsUser, &QAction::triggered, this, [this, hasOwnerRole]() {
            Ruqola::self()->rocketChatAccount()->changeRoles(mRoom->roomId(), mUserId, mRoom->channelType(), hasOwnerRole ? RocketChatAccount::RemoveOwner : RocketChatAccount::AddOwner);
        });

        menu.addAction(removeAsUser);

        const bool hasLeaderRole = mRoom->userHasLeaderRole(mUserId);
        auto *removeAsLeader = new QAction(hasLeaderRole ? i18n("Remove as Leader") : i18n("Add as Leader"), &menu);
        connect(removeAsLeader, &QAction::triggered, this, [this, hasLeaderRole]() {
            Ruqola::self()->rocketChatAccount()->changeRoles(mRoom->roomId(), mUserId, mRoom->channelType(), hasLeaderRole ? RocketChatAccount::RemoveLeader : RocketChatAccount::AddLeader);
        });
        menu.addAction(removeAsLeader);

        const bool hasModeratorRole = mRoom->userHasModeratorRole(mUserId);
        auto *removeAsModerator = new QAction(hasModeratorRole ? i18n("Remove as Moderator") : i18n("Add as Moderator"), &menu);
        connect(removeAsModerator, &QAction::triggered, this, [this, hasModeratorRole]() {
            Ruqola::self()->rocketChatAccount()->changeRoles(mRoom->roomId(), mUserId, mRoom->channelType(), hasModeratorRole ? RocketChatAccount::RemoveModerator : RocketChatAccount::AddModerator);
        });
        menu.addAction(removeAsModerator);

        auto *removeFromRoom = new QAction(i18n("Remove from Room"), &menu);
        connect(removeFromRoom, &QAction::triggered, this, &UsersInRoomMenu::slotRemoveFromRoom);
        menu.addAction(removeFromRoom);
    }
    if (isNotMe) {
        if (isAdirectChannel) {
            if (!menu.isEmpty()) {
                menu.addSeparator();
            }
            const bool userIsBlocked = mRoom->blocker();
            auto *blockAction = new QAction(userIsBlocked ? i18n("Unblock User") : i18n("Block User"), &menu);
            connect(blockAction, &QAction::triggered, this, &UsersInRoomMenu::slotBlockUser);
            menu.addAction(blockAction);
        } else {
            if (!menu.isEmpty()) {
                menu.addSeparator();
            }

            const bool userIsIgnored = mRoom->userIsIgnored(mUserId);
            auto *ignoreAction = new QAction(userIsIgnored ? i18n("Unignore") : i18n("Ignore"), &menu);
            connect(ignoreAction, &QAction::triggered, this, &UsersInRoomMenu::slotIgnoreUser);
            menu.addAction(ignoreAction);
        }
    }
    menu.exec(mParentWidget->mapToGlobal(pos));
}

void UsersInRoomMenu::slotUserInfo()
{
    DirectChannelInfoDialog dlg(mParentWidget);
    dlg.setUserName(mUserName);
    dlg.exec();
}

void UsersInRoomMenu::setParentWidget(QWidget *parentWidget)
{
    mParentWidget = parentWidget;
}

void UsersInRoomMenu::setUserId(const QString &userId)
{
    mUserId = userId;
}

void UsersInRoomMenu::setUserName(const QString &userName)
{
    mUserName = userName;
}

void UsersInRoomMenu::setRoom(Room *room)
{
    mRoom = room;
}
