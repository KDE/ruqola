/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroommenu.h"
#include "dialogs/directchannelinfodialog.h"
#include "rocketchataccount.h"
#include "roomutil.h"
#include "ruqola.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QAction>
#include <QMenu>

UsersInRoomMenu::UsersInRoomMenu(QObject *parent)
    : QObject(parent)
{
}

UsersInRoomMenu::~UsersInRoomMenu() = default;

void UsersInRoomMenu::slotOpenConversation()
{
    Q_EMIT Ruqola::self()->rocketChatAccount()->openLinkRequested(RoomUtil::generateUserLink(mUserName));
}

void UsersInRoomMenu::slotBlockUser()
{
    const bool userIsBlocked = mRoom->blocker();
    if (!userIsBlocked) {
        if (KMessageBox::ButtonCode::SecondaryAction
            == KMessageBox::questionTwoActions(mParentWidget,
                                               i18n("Do you want to block this user?"),
                                               i18nc("@title", "Block User"),
                                               KGuiItem(i18nc("@action:button", "Block User"), QStringLiteral("dialog-ok")),
                                               KStandardGuiItem::cancel())) {
            return;
        }
    }
    Ruqola::self()->rocketChatAccount()->blockUser(mRoom->roomId(), !userIsBlocked);
}

void UsersInRoomMenu::slotIgnoreUser()
{
    const bool userIsIgnored = mRoom->userIsIgnored(mUserId);
    if (!userIsIgnored) {
        if (KMessageBox::ButtonCode::SecondaryAction
            == KMessageBox::questionTwoActions(mParentWidget,
                                               i18n("Do you want to ignore this user?"),
                                               i18nc("@title", "Ignore User"),
                                               KGuiItem(i18nc("@action:button", "Ignore User"), QStringLiteral("dialog-ok")),
                                               KStandardGuiItem::cancel())) {
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
    const bool isAdministrator = Ruqola::self()->rocketChatAccount()->ownUser().isAdministrator();
    auto account = Ruqola::self()->rocketChatAccount();
    const QString ownUserId = account->userId();
    const bool isAdirectChannel = mRoom->channelType() == Room::RoomType::Direct;
    const bool isNotMe = mUserId != ownUserId;
    QMenu menu(mParentWidget);

    if (account->hasPermission(QStringLiteral("create-d"))) {
        if (isNotMe && !isAdirectChannel) {
            auto conversationAction = new QAction(i18n("Start Conversation"), &menu);
            connect(conversationAction, &QAction::triggered, this, &UsersInRoomMenu::slotOpenConversation);
            menu.addAction(conversationAction);
        }
    }
    if (!menu.isEmpty()) {
        menu.addSeparator();
    }
    auto userInfoAction = new QAction(QIcon::fromTheme(QStringLiteral("documentinfo")), i18n("User Info"), &menu);
    connect(userInfoAction, &QAction::triggered, this, &UsersInRoomMenu::slotUserInfo);
    menu.addAction(userInfoAction);
    if ((isAdministrator || canManageUsersInRoom) && !isAdirectChannel) {
        if (!menu.isEmpty()) {
            menu.addSeparator();
        }
        if (isAdministrator || mRoom->hasPermission(QStringLiteral("set-owner"))) {
            const bool hasOwnerRole = mRoom->userHasOwnerRole(mUserId);
            auto removeAsOwner = new QAction(hasOwnerRole ? i18n("Remove as Owner") : i18n("Add as Owner"), &menu);
            connect(removeAsOwner, &QAction::triggered, this, [this, hasOwnerRole, account]() {
                account->changeRoles(mRoom->roomId(),
                                     mUserId,
                                     mRoom->channelType(),
                                     hasOwnerRole ? RocketChatAccount::RemoveOwner : RocketChatAccount::AddOwner);
            });

            menu.addAction(removeAsOwner);
        }

        if (isAdministrator || mRoom->hasPermission(QStringLiteral("set-leader"))) {
            const bool hasLeaderRole = mRoom->userHasLeaderRole(mUserId);
            auto removeAsLeader = new QAction(hasLeaderRole ? i18n("Remove as Leader") : i18n("Add as Leader"), &menu);
            connect(removeAsLeader, &QAction::triggered, this, [this, hasLeaderRole, account]() {
                account->changeRoles(mRoom->roomId(),
                                     mUserId,
                                     mRoom->channelType(),
                                     hasLeaderRole ? RocketChatAccount::RemoveLeader : RocketChatAccount::AddLeader);
            });
            menu.addAction(removeAsLeader);
        }

        if (isAdministrator || mRoom->hasPermission(QStringLiteral("set-moderator"))) {
            const bool hasModeratorRole = mRoom->userHasModeratorRole(mUserId);
            auto removeAsModerator = new QAction(hasModeratorRole ? i18n("Remove as Moderator") : i18n("Add as Moderator"), &menu);
            connect(removeAsModerator, &QAction::triggered, this, [this, hasModeratorRole, account]() {
                account->changeRoles(mRoom->roomId(),
                                     mUserId,
                                     mRoom->channelType(),
                                     hasModeratorRole ? RocketChatAccount::RemoveModerator : RocketChatAccount::AddModerator);
            });
            menu.addAction(removeAsModerator);
        }
        if (isAdministrator || mRoom->hasPermission(QStringLiteral("remove-user"))) {
            menu.addSeparator();
            auto removeFromRoom = new QAction(i18n("Remove from Room"), &menu);
            connect(removeFromRoom, &QAction::triggered, this, &UsersInRoomMenu::slotRemoveFromRoom);
            menu.addAction(removeFromRoom);
        }
    }
    if (isNotMe) {
        if (isAdirectChannel) {
            if (!menu.isEmpty()) {
                menu.addSeparator();
            }
            const bool userIsBlocked = mRoom->blocker();
            auto blockAction = new QAction(userIsBlocked ? i18n("Unblock User") : i18n("Block User"), &menu);
            connect(blockAction, &QAction::triggered, this, &UsersInRoomMenu::slotBlockUser);
            menu.addAction(blockAction);
        } else {
            if (!menu.isEmpty()) {
                menu.addSeparator();
            }

            const bool userIsIgnored = mRoom->userIsIgnored(mUserId);
            auto ignoreAction = new QAction(userIsIgnored ? i18n("Unignore") : i18n("Ignore"), &menu);
            connect(ignoreAction, &QAction::triggered, this, &UsersInRoomMenu::slotIgnoreUser);
            menu.addAction(ignoreAction);
        }
    }
    menu.exec(mParentWidget->mapToGlobal(pos));
}

void UsersInRoomMenu::slotUserInfo()
{
    auto rcAccount = Ruqola::self()->rocketChatAccount();
    DirectChannelInfoDialog dlg(rcAccount, mParentWidget);
    dlg.setUserName(mUserName);
    dlg.setRoles(rcAccount->roleInfo());
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

#include "moc_usersinroommenu.cpp"
