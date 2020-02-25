/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "channelactionpopupmenu.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QMenu>

ChannelActionPopupMenu::ChannelActionPopupMenu(QObject *parent)
    : QObject(parent)
{
    mMenu = new QMenu;
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

    mShowSnipperedMessages = new QAction(i18n("Show Snippered Messages..."), this);
    mMenu->addAction(mShowSnipperedMessages);
    connect(mShowSnipperedMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowSnippered);
    });

    mShowFileAttachments = new QAction(QIcon::fromTheme(QStringLiteral("document-send-symbolic")), i18n("Show File Attachment..."), this);
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

    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    mMenu->addAction(separator);

    mConfigureNotification = new QAction(QIcon::fromTheme(QStringLiteral("preferences-desktop-notification")), i18n("Configure Notification..."), this);
    mMenu->addAction(mConfigureNotification);
    connect(mConfigureNotification, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::Notification);
    });
}

QMenu *ChannelActionPopupMenu::menu() const
{
    return mMenu;
}

void ChannelActionPopupMenu::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mCurrentRocketChatAccount = account;
}

void ChannelActionPopupMenu::slotUpdateMenu()
{
    mShowPinnedMessages->setVisible(mCurrentRocketChatAccount->hasPinnedMessagesSupport() && mCurrentRocketChatAccount->allowMessagePinningEnabled());
    mShowStarredMessages->setVisible(mCurrentRocketChatAccount->hasStarredMessagesSupport() && mCurrentRocketChatAccount->allowMessageStarringEnabled());
    mShowSnipperedMessages->setVisible(mCurrentRocketChatAccount->hasSnippetedMessagesSupport() && mCurrentRocketChatAccount->allowMessageSnippetingEnabled());
}
