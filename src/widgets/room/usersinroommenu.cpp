/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroommenu.h"
using namespace Qt::Literals::StringLiterals;

#include "connection.h"
#include "dialogs/directchannelinfodialog.h"
#include "dialogs/reportuserdialog.h"
#include "moderation/moderationreportuserjob.h"
#include "rocketchataccount.h"
#include "roomutil.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
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
    Ruqola::self()->rocketChatAccount()->blockUser(QString::fromLatin1(mRoom->roomId()), !userIsBlocked);
}

void UsersInRoomMenu::slotReportUser()
{
    ReportUserDialog dlg(mParentWidget);
    dlg.setUserName(mUserName);
    if (dlg.exec()) {
        auto job = new RocketChatRestApi::ModerationReportUserJob(this);
        Ruqola::self()->rocketChatAccount()->restApi()->initializeRestApiJob(job);
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
                                               KGuiItem(i18nc("@action:button", "Mute User"), QStringLiteral("dialog-ok")),
                                               KStandardGuiItem::cancel())) {
            return;
        }
    }
    // Fix remove Ruqola::self()
    Ruqola::self()->rocketChatAccount()->muteUser(mRoom->roomId(), mUserName, !userIsMuted);
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
    const QByteArray ownUserId = account->userId();
    const bool isAdirectChannel = mRoom->channelType() == Room::RoomType::Direct;
    const bool isNotMe = mUserId != ownUserId;
    QMenu menu(mParentWidget);

    if (account->hasPermission(QStringLiteral("create-d"))) {
        if (isNotMe && !isAdirectChannel) {
            auto conversationAction = new QAction(i18nc("@action", "Start Conversation"), &menu);
            connect(conversationAction, &QAction::triggered, this, &UsersInRoomMenu::slotOpenConversation);
            menu.addAction(conversationAction);
        }
    }
    if (!menu.isEmpty()) {
        menu.addSeparator();
    }
    auto userInfoAction = new QAction(QIcon::fromTheme(QStringLiteral("documentinfo")), i18nc("@action", "User Info"), &menu);
    connect(userInfoAction, &QAction::triggered, this, &UsersInRoomMenu::slotUserInfo);
    menu.addAction(userInfoAction);
    if ((isAdministrator || canManageUsersInRoom) && !isAdirectChannel) {
        if (!menu.isEmpty()) {
            menu.addSeparator();
        }
        if (isAdministrator || mRoom->hasPermission(QStringLiteral("set-owner"))) {
            const bool hasOwnerRole = mRoom->userHasOwnerRole(mUserId);
            auto removeAsOwner = new QAction(hasOwnerRole ? i18nc("@action", "Remove as Owner") : i18nc("@action", "Add as Owner"), &menu);
            connect(removeAsOwner, &QAction::triggered, this, [this, hasOwnerRole, account]() {
                account->changeRoles(mRoom->roomId(),
                                     QString::fromLatin1(mUserId),
                                     mRoom->channelType(),
                                     hasOwnerRole ? RocketChatAccount::RoleType::RemoveOwner : RocketChatAccount::RoleType::AddOwner);
            });

            menu.addAction(removeAsOwner);
        }

        if (isAdministrator || mRoom->hasPermission(QStringLiteral("set-leader"))) {
            const bool hasLeaderRole = mRoom->userHasLeaderRole(mUserId);
            auto removeAsLeader = new QAction(hasLeaderRole ? i18nc("@action", "Remove as Leader") : i18nc("@action", "Add as Leader"), &menu);
            connect(removeAsLeader, &QAction::triggered, this, [this, hasLeaderRole, account]() {
                account->changeRoles(mRoom->roomId(),
                                     QString::fromLatin1(mUserId),
                                     mRoom->channelType(),
                                     hasLeaderRole ? RocketChatAccount::RoleType::RemoveLeader : RocketChatAccount::RoleType::AddLeader);
            });
            menu.addAction(removeAsLeader);
        }

        if (isAdministrator || mRoom->hasPermission(QStringLiteral("set-moderator"))) {
            const bool hasModeratorRole = mRoom->userHasModeratorRole(mUserId);
            auto removeAsModerator = new QAction(hasModeratorRole ? i18nc("@action", "Remove as Moderator") : i18nc("@action", "Add as Moderator"), &menu);
            connect(removeAsModerator, &QAction::triggered, this, [this, hasModeratorRole, account]() {
                account->changeRoles(mRoom->roomId(),
                                     QString::fromLatin1(mUserId),
                                     mRoom->channelType(),
                                     hasModeratorRole ? RocketChatAccount::RoleType::RemoveModerator : RocketChatAccount::RoleType::AddModerator);
            });
            menu.addAction(removeAsModerator);
        }
        if (isAdministrator || mRoom->hasPermission(QStringLiteral("remove-user"))) {
            menu.addSeparator();
            auto removeFromRoom = new QAction(i18nc("@action", "Remove from Room"), &menu);
            connect(removeFromRoom, &QAction::triggered, this, &UsersInRoomMenu::slotRemoveFromRoom);
            menu.addAction(removeFromRoom);
        }
    }
    if (isNotMe) {
        if (!menu.isEmpty()) {
            menu.addSeparator();
        }
        if (isAdirectChannel) {
            const bool userIsBlocked = mRoom->blocker();
            auto blockAction = new QAction(userIsBlocked ? i18nc("@action", "Unblock User") : i18nc("@action", "Block User"), &menu);
            connect(blockAction, &QAction::triggered, this, &UsersInRoomMenu::slotBlockUser);
            menu.addAction(blockAction);
        } else {
            const bool userIsIgnored = mRoom->userIsIgnored(mUserId);
            auto ignoreAction = new QAction(userIsIgnored ? i18nc("@action", "Unignore") : i18nc("@action", "Ignore"), &menu);
            connect(ignoreAction, &QAction::triggered, this, &UsersInRoomMenu::slotIgnoreUser);
            menu.addAction(ignoreAction);
            menu.addSeparator();
            if (mRoom->hasPermission(QStringLiteral("mute-user"))) {
                const bool userIsMuted = mRoom->userIsMuted(mUserName);
                auto muteAction = new QAction(userIsMuted ? i18nc("@action", "Unmute User") : i18nc("@action", "Mute User"), &menu);
                muteAction->setIcon(userIsMuted ? QIcon::fromTheme("mic-on"_L1) : QIcon::fromTheme("mic-off"_L1));
                connect(muteAction, &QAction::triggered, this, &UsersInRoomMenu::slotMuteUser);
                menu.addAction(muteAction);
            }
        }

        menu.addSeparator();
        auto reportUserAction = new QAction(QIcon::fromTheme("emblem-warning"_L1), i18nc("@action", "Report User"), &menu);
        connect(reportUserAction, &QAction::triggered, this, &UsersInRoomMenu::slotReportUser);
        menu.addAction(reportUserAction);
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
