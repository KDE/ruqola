/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelactionpopupmenu.h"
#include "rocketchataccount.h"
#include "room.h"
#include <KLocalizedString>
#include <QMenu>

ChannelActionPopupMenu::ChannelActionPopupMenu(QObject *parent)
    : QObject(parent)
    , mMenu(new QMenu)
{
    mMenu->setObjectName(QStringLiteral("mMenu"));
    connect(mMenu, &QMenu::aboutToShow, this, &ChannelActionPopupMenu::slotUpdateMenu);
    createMenu();
}

ChannelActionPopupMenu::~ChannelActionPopupMenu()
{
    delete mMenu;
}

void ChannelActionPopupMenu::createMenu()
{
    mPruneMessages = new QAction(i18n("Prune Messages..."), this);
    mMenu->addAction(mPruneMessages);
    connect(mPruneMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::PruneMessages);
    });

    mPruneMessagesSeparator = mMenu->addSeparator();

    mShowMentions = new QAction(i18n("Show Mentions..."), this);
    mMenu->addAction(mShowMentions);
    connect(mShowMentions, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowMentions);
    });

    mShowPinnedMessages = new QAction(i18n("Show Pinned Messages..."), this);
    mMenu->addAction(mShowPinnedMessages);
    connect(mShowPinnedMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowPinned);
    });

    mShowStarredMessages = new QAction(i18n("Show Starred Messages..."), this);
    mMenu->addAction(mShowStarredMessages);
    connect(mShowStarredMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowStarred);
    });

    mShowFileAttachments = new QAction(QIcon::fromTheme(QStringLiteral("document-send-symbolic")), i18n("Show File Attachments..."), this);
    mMenu->addAction(mShowFileAttachments);
    connect(mShowFileAttachments, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowAttachment);
    });

    mShowDiscussions = new QAction(i18n("Show Discussions..."), this);
    mMenu->addAction(mShowDiscussions);
    connect(mShowDiscussions, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowDiscussions);
    });

    mShowThreads = new QAction(i18n("Show Threads..."), this);
    mMenu->addAction(mShowThreads);
    connect(mShowThreads, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowThreads);
    });

    mMenu->addSeparator();

    mConfigureNotification = new QAction(QIcon::fromTheme(QStringLiteral("preferences-desktop-notification")), i18n("Configure Notification..."), this);
    mMenu->addAction(mConfigureNotification);
    connect(mConfigureNotification, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::Notification);
    });

    mAutoTranslateSeparator = mMenu->addSeparator();
    mAutoTranslate = new QAction(i18n("Configure Auto-Translate..."), this);
    mMenu->addAction(mAutoTranslate);
    connect(mAutoTranslate, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::AutoTranslate);
    });

    mInviteUsersGenerateUrlSeparator = mMenu->addSeparator();
    mMenu->addAction(mInviteUsersGenerateUrlSeparator);
    mInviteUsersGenerateUrl = new QAction(i18n("Invite Users"), this);
    mMenu->addAction(mInviteUsersGenerateUrl);
    connect(mInviteUsersGenerateUrl, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::InviteUsers);
    });

    mAddUserInRoomsSeparator = mMenu->addSeparator();
    mAddUserInRooms = new QAction(i18n("Add Users in Channel..."), this);
    mMenu->addAction(mAddUserInRoomsSeparator);
    connect(mAddUserInRooms, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::AddUsersInRoom);
    });
    mMenu->addAction(mAddUserInRooms);

    mMenu->addSeparator();
    mStartVideoChat = new QAction(QIcon::fromTheme(QStringLiteral("camera-video")), i18n("Video Chat"), this);
    mMenu->addAction(mStartVideoChat);
    connect(mStartVideoChat, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::VideoChat);
    });

    mMenu->addSeparator();
    mExportMessages = new QAction(i18n("Export Messages..."), this);
    mMenu->addAction(mExportMessages);
    connect(mExportMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ExportMessages);
    });

    mMenu->addSeparator();
    mOffTheRecordMessages = new QAction(i18n("OTR"), this);
    mOffTheRecordMessages->setCheckable(true);
    mMenu->addAction(mOffTheRecordMessages);
    connect(mOffTheRecordMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::OtrMessages);
    });
    mMenu->addSeparator();
    mEncryptMessages = new QAction(i18n("Encrypt Messages"), this);
    mEncryptMessages->setCheckable(true);
    mMenu->addAction(mEncryptMessages);
    connect(mEncryptMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::EncryptMessages);
    });
}

QAction *ChannelActionPopupMenu::createSeparator()
{
    return mMenu->addSeparator();
}

QMenu *ChannelActionPopupMenu::menu() const
{
    return mMenu;
}

void ChannelActionPopupMenu::setRoom(Room *room)
{
    mRoom = room;
}

void ChannelActionPopupMenu::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mCurrentRocketChatAccount = account;
}

void ChannelActionPopupMenu::slotUpdateMenu()
{
    mShowPinnedMessages->setVisible(mCurrentRocketChatAccount->allowMessagePinningEnabled());
    mShowStarredMessages->setVisible(mCurrentRocketChatAccount->allowMessageStarringEnabled());
    mAutoTranslate->setVisible(mCurrentRocketChatAccount->hasAutotranslateSupport());
    mAutoTranslateSeparator->setVisible(mCurrentRocketChatAccount->autoTranslateEnabled());

    bool hasPermissionInviteUserSupport = mRoom && mRoom->hasPermission(QStringLiteral("create-invite-links"));
    mInviteUsersGenerateUrl->setVisible(hasPermissionInviteUserSupport);
    mInviteUsersGenerateUrlSeparator->setVisible(hasPermissionInviteUserSupport);
    mStartVideoChat->setVisible(mCurrentRocketChatAccount->jitsiEnabled());

    mAddUserInRoomsSeparator->setVisible(mRoom && mRoom->canBeModify());
    mAddUserInRooms->setVisible(mRoom && mRoom->canBeModify());

    const bool showPruneMessage = mCurrentRocketChatAccount->hasPermission(QStringLiteral("clean-channel-history"));
    mPruneMessages->setVisible(showPruneMessage);
    mPruneMessagesSeparator->setVisible(showPruneMessage);

    mExportMessages->setVisible(mCurrentRocketChatAccount->hasPermission(QStringLiteral("mail-messages")));

    // FIXME Disable for the moment
    mOffTheRecordMessages->setVisible(false && mCurrentRocketChatAccount->otrEnabled() && mRoom->channelType() == Room::RoomType::Direct);

    // FIXME Disable for the moment
    // TODO
    mEncryptMessages->setVisible(false);
}

#include "moc_channelactionpopupmenu.cpp"
