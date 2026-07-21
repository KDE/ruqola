/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroommenu.h"

#include "connection.h"
#include "dialogs/directchannelinfodialog.h"
#include "dialogs/reportuserdialog.h"
#include "moderation/moderationreportuserjob.h"
#include "rocketchataccount.h"
#include "rooms/roomsbanuserjob.h"
#include "roomutil.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QAction>
#include <QMenu>

using namespace Qt::Literals::StringLiterals;
UsersInRoomMenu::UsersInRoomMenu(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
}

UsersInRoomMenu::~UsersInRoomMenu() = default;

void UsersInRoomMenu::slotOpenConversation()
{
    Q_EMIT mRocketChatAccount->openLinkRequested(RoomUtil::generateUserLink(mUserName));
}

void UsersInRoomMenu::slotBlockUser()
{
    const bool userIsBlocked = mRoom->blocker();
    if (!userIsBlocked) {
        if (KMessageBox::ButtonCode::SecondaryAction
            == KMessageBox::questionTwoActions(mParentWidget,
                                               i18n("Do you want to block this user?"),
                                               i18nc("@title", "Block User"),
                                               KGuiItem(i18nc("@action:button", "Block User"), u"dialog-ok"_s),
                                               KStandardGuiItem::cancel())) {
            return;
        }
    }
    mRocketChatAccount->blockUser(QString::fromLatin1(mRoom->roomId()), !userIsBlocked);
}

void UsersInRoomMenu::slotReportUser()
{
    ReportUserDialog dlg(mParentWidget);
    dlg.setUserName(mUserName);
    if (dlg.exec()) {
        auto job = new RocketChatRestApi::ModerationReportUserJob(this);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        job->setDescription(dlg.message());
        job->setReportedUserId(mUserId);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ModerationReportUserJob job";
        }
    }
}

void UsersInRoomMenu::slotMuteUser()
{
    const bool userIsMuted = mRoom->userIsMuted(mUserName);
    if (!userIsMuted) {
        if (KMessageBox::ButtonCode::SecondaryAction
            == KMessageBox::questionTwoActions(mParentWidget,
                                               i18n("Do you want to mute this user?"),
                                               i18nc("@title", "Mute User"),
                                               KGuiItem(i18nc("@action:button", "Mute User"), u"dialog-ok"_s),
                                               KStandardGuiItem::cancel())) {
            return;
        }
    }
    mRocketChatAccount->muteUser(mRoom->roomId(), mUserName, !userIsMuted);
}

void UsersInRoomMenu::slotIgnoreUser()
{
    const bool userIsIgnored = mRoom->userIsIgnored(mUserId);
    if (!userIsIgnored) {
        if (KMessageBox::ButtonCode::SecondaryAction
            == KMessageBox::questionTwoActions(mParentWidget,
                                               i18n("Do you want to ignore this user?"),
                                               i18nc("@title", "Ignore User"),
                                               KGuiItem(i18nc("@action:button", "Ignore User"), u"dialog-ok"_s),
                                               KStandardGuiItem::cancel())) {
            return;
        }
    }
    mRocketChatAccount->ignoreUser(mRoom->roomId(), mUserId, !userIsIgnored);
}

void UsersInRoomMenu::slotRemoveFromRoom()
{
    if (KMessageBox::ButtonCode::SecondaryAction
        == KMessageBox::questionTwoActions(mParentWidget,
                                           i18n("Do you want to remove this user from Channel?"),
                                           i18nc("@title", "Remove User"),
                                           KGuiItem(i18nc("@action:button", "Remove User"), u"dialog-ok"_s),
                                           KStandardGuiItem::cancel())) {
        return;
    }
    mRocketChatAccount->kickUser(mRoom->roomId(), mUserId, mRoom->channelType());
}

QMenu *UsersInRoomMenu::createUserInRoomMenu()
{
    mMenu = new QMenu(mParentWidget);
    return mMenu;
}

void UsersInRoomMenu::updateUserInRoomMenu()
{
    if (!mMenu) {
        mMenu = new QMenu(mParentWidget);
    } else {
        mMenu->clear();
    }

    const bool canManageUsersInRoom = mRoom->canChangeRoles();
    const bool isAdministrator = mRocketChatAccount->ownUser().isAdministrator();
    const QByteArray ownUserId = mRocketChatAccount->userId();
    const bool isAdirectChannel = mRoom->channelType() == Room::RoomType::Direct;
    const bool isNotMe = mUserId != ownUserId;

    if (mRocketChatAccount->hasPermission(u"create-d"_s)) {
        if (isNotMe && !isAdirectChannel) {
            auto conversationAction = new QAction(i18nc("@action", "Start Conversation"), mMenu);
            connect(conversationAction, &QAction::triggered, this, &UsersInRoomMenu::slotOpenConversation);
            mMenu->addAction(conversationAction);
        }
    }
    if (!mMenu->isEmpty()) {
        mMenu->addSeparator();
    }
    auto userInfoAction = new QAction(QIcon::fromTheme(u"documentinfo"_s), i18nc("@action", "User Info"), mMenu);
    connect(userInfoAction, &QAction::triggered, this, &UsersInRoomMenu::slotUserInfo);
    mMenu->addAction(userInfoAction);
    if ((isAdministrator || canManageUsersInRoom) && !isAdirectChannel) {
        if (!mMenu->isEmpty()) {
            mMenu->addSeparator();
        }
        if (isAdministrator || mRoom->hasPermission(u"set-owner"_s)) {
            const bool hasOwnerRole = mRoom->userHasOwnerRole(mUserId);
            auto removeAsOwner = new QAction(hasOwnerRole ? i18nc("@action", "Remove as Owner") : i18nc("@action", "Add as Owner"), mMenu);
            connect(removeAsOwner, &QAction::triggered, this, [this, hasOwnerRole]() {
                mRocketChatAccount->changeRoles(mRoom->roomId(),
                                                QString::fromLatin1(mUserId),
                                                mRoom->channelType(),
                                                hasOwnerRole ? RocketChatAccount::RoleType::RemoveOwner : RocketChatAccount::RoleType::AddOwner);
            });

            mMenu->addAction(removeAsOwner);
        }

        if (isAdministrator || mRoom->hasPermission(u"set-leader"_s)) {
            const bool hasLeaderRole = mRoom->userHasLeaderRole(mUserId);
            auto removeAsLeader = new QAction(hasLeaderRole ? i18nc("@action", "Remove as Leader") : i18nc("@action", "Add as Leader"), mMenu);
            connect(removeAsLeader, &QAction::triggered, this, [this, hasLeaderRole]() {
                mRocketChatAccount->changeRoles(mRoom->roomId(),
                                                QString::fromLatin1(mUserId),
                                                mRoom->channelType(),
                                                hasLeaderRole ? RocketChatAccount::RoleType::RemoveLeader : RocketChatAccount::RoleType::AddLeader);
            });
            mMenu->addAction(removeAsLeader);
        }

        if (isAdministrator || mRoom->hasPermission(u"set-moderator"_s)) {
            const bool hasModeratorRole = mRoom->userHasModeratorRole(mUserId);
            auto removeAsModerator = new QAction(hasModeratorRole ? i18nc("@action", "Remove as Moderator") : i18nc("@action", "Add as Moderator"), mMenu);
            connect(removeAsModerator, &QAction::triggered, this, [this, hasModeratorRole]() {
                mRocketChatAccount->changeRoles(mRoom->roomId(),
                                                QString::fromLatin1(mUserId),
                                                mRoom->channelType(),
                                                hasModeratorRole ? RocketChatAccount::RoleType::RemoveModerator : RocketChatAccount::RoleType::AddModerator);
            });
            mMenu->addAction(removeAsModerator);
        }
        if (isAdministrator || mRoom->hasPermission(u"remove-user"_s)) {
            mMenu->addSeparator();
            auto removeFromRoom = new QAction(i18nc("@action", "Remove from Room"), mMenu);
            connect(removeFromRoom, &QAction::triggered, this, &UsersInRoomMenu::slotRemoveFromRoom);
            mMenu->addAction(removeFromRoom);
        }
    }
    if (isNotMe) {
        if (!mMenu->isEmpty()) {
            mMenu->addSeparator();
        }
        if (isAdirectChannel) {
            const bool userIsBlocked = mRoom->blocker();
            auto blockAction = new QAction(userIsBlocked ? i18nc("@action", "Unblock User") : i18nc("@action", "Block User"), mMenu);
            connect(blockAction, &QAction::triggered, this, &UsersInRoomMenu::slotBlockUser);
            mMenu->addAction(blockAction);
        } else {
            const bool userIsIgnored = mRoom->userIsIgnored(mUserId);
            auto ignoreAction = new QAction(userIsIgnored ? i18nc("@action", "Unignore") : i18nc("@action", "Ignore"), mMenu);
            connect(ignoreAction, &QAction::triggered, this, &UsersInRoomMenu::slotIgnoreUser);
            mMenu->addAction(ignoreAction);
            mMenu->addSeparator();
            if (mRoom->hasPermission(u"mute-user"_s)) {
                const bool userIsMuted = mRoom->userIsMuted(mUserName);
                auto muteAction = new QAction(userIsMuted ? i18nc("@action", "Unmute User") : i18nc("@action", "Mute User"), mMenu);
                muteAction->setIcon(userIsMuted ? QIcon::fromTheme("mic-on"_L1) : QIcon::fromTheme("mic-off"_L1));
                connect(muteAction, &QAction::triggered, this, &UsersInRoomMenu::slotMuteUser);
                mMenu->addAction(muteAction);
            }
        }

        mMenu->addSeparator();
        auto reportUserAction = new QAction(QIcon::fromTheme("emblem-warning"_L1), i18nc("@action", "Report User"), mMenu);
        connect(reportUserAction, &QAction::triggered, this, &UsersInRoomMenu::slotReportUser);
        mMenu->addAction(reportUserAction);

        if (mRoom->hasPermission(u"ban-user"_s)) {
            mMenu->addSeparator();
            auto banUserFromRoomAction = new QAction(QIcon::fromTheme("im-ban-user"_L1), i18nc("@action", "Ban User From Room"), mMenu);
            connect(banUserFromRoomAction, &QAction::triggered, this, &UsersInRoomMenu::slotBanUserFromRoomAction);
            mMenu->addAction(banUserFromRoomAction);
        }
    }
}

void UsersInRoomMenu::slotCustomContextMenuRequested(const QPoint &pos)
{
    const bool offline = mRocketChatAccount->offlineMode();
    if (offline) {
        return;
    }
    updateUserInRoomMenu();

    mMenu->exec(mParentWidget->mapToGlobal(pos));
}

void UsersInRoomMenu::slotBanUserFromRoomAction()
{
    if (KMessageBox::ButtonCode::SecondaryAction
        == KMessageBox::questionTwoActions(mParentWidget,
                                           i18n("Do you want to ban this user?"),
                                           i18nc("@title", "Ban User"),
                                           KGuiItem(i18nc("@action:button", "Ban User"), u"dialog-ok"_s),
                                           KStandardGuiItem::cancel())) {
        return;
    }
    auto job = new RocketChatRestApi::RoomsBanUserJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    job->setRoomId(mRoom->roomId());
    job->setUserName(mUserName);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start RoomsBanUserJob job";
    }
}

void UsersInRoomMenu::slotUserInfo()
{
    DirectChannelInfoDialog dlg(mRocketChatAccount, mParentWidget);
    const DirectChannelInfoWidget::DirectChannelInfo info{
        .userName = mUserName,
        .roles = mRocketChatAccount->roleInfo(),
        .room = mRoom,
    };
    dlg.setDirectChannelInfo(info);
    dlg.exec();
}

void UsersInRoomMenu::setParentWidget(QWidget *parentWidget)
{
    mParentWidget = parentWidget;
}

void UsersInRoomMenu::setUserId(const QByteArray &userId)
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
