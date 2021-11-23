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

#include "roomheaderwidget.h"
#include "channelactionpopupmenu.h"
#include "room.h"
#include "roomheaderlabel.h"
#include "ruqolawidgets_debug.h"
#include "teamnamelabel.h"
#include <KLocalizedString>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>

RoomHeaderWidget::RoomHeaderWidget(QWidget *parent)
    : QWidget(parent)
    , mRoomName(new QLabel(this))
    , mTeamName(new TeamNameLabel(this))
    , mRoomHeaderLabel(new RoomHeaderLabel(this))
    , mFavoriteButton(new QToolButton(this))
    , mEncryptedButton(new QToolButton(this))
    , mDiscussionBackButton(new QToolButton(this))
    , mListOfUsersButton(new QToolButton(this))
    , mSearchMessageButton(new QToolButton(this))
    , mChannelAction(new QToolButton(this))
    , mChannelInfoButton(new QToolButton(this))
    , mTeamChannelsButton(new QToolButton(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto headerLayout = new QHBoxLayout;
    headerLayout->setObjectName(QStringLiteral("headerLayout"));
    headerLayout->setContentsMargins({});
    mainLayout->addLayout(headerLayout);

    mFavoriteButton->setAutoRaise(true);
    mFavoriteButton->setObjectName(QStringLiteral("mFavoriteButton"));
    mFavoriteButton->setIcon(QIcon::fromTheme(QStringLiteral("favorite")));
    mFavoriteButton->setCheckable(true);
    mFavoriteButton->setToolTip(i18n("Favorite"));
    headerLayout->addWidget(mFavoriteButton, 0, Qt::AlignTop);
    connect(mFavoriteButton, &QToolButton::clicked, this, &RoomHeaderWidget::favoriteChanged);

    mDiscussionBackButton->setAutoRaise(true);
    mDiscussionBackButton->setObjectName(QStringLiteral("mDiscussionBackButton"));
    mDiscussionBackButton->setIcon(QIcon::fromTheme(QStringLiteral("draw-arrow-back")));
    mDiscussionBackButton->setCheckable(false);
    headerLayout->addWidget(mDiscussionBackButton, 0, Qt::AlignTop);
    connect(mDiscussionBackButton, &QToolButton::clicked, this, &RoomHeaderWidget::goBackToRoom);
    mDiscussionBackButton->setVisible(false);

    mEncryptedButton->setAutoRaise(true);
    mEncryptedButton->setObjectName(QStringLiteral("mEncryptedButton"));
    mEncryptedButton->setIcon(QIcon::fromTheme(QStringLiteral("encrypted")));
    mEncryptedButton->setToolTip(i18n("E2E Enabled"));
    mEncryptedButton->setCheckable(true);
    mEncryptedButton->setVisible(false);
    headerLayout->addWidget(mEncryptedButton, 0, Qt::AlignTop);
    connect(mEncryptedButton, &QToolButton::clicked, this, &RoomHeaderWidget::encryptedChanged);

    auto infoLayout = new QVBoxLayout;
    infoLayout->setObjectName(QStringLiteral("infoLayout"));
    infoLayout->setContentsMargins({});
    headerLayout->addLayout(infoLayout);

    auto roomNameLayout = new QHBoxLayout;
    roomNameLayout->setObjectName(QStringLiteral("roomNameLayout"));
    roomNameLayout->setContentsMargins({});
    roomNameLayout->setSpacing(0);
    infoLayout->addLayout(roomNameLayout);

    mRoomName->setObjectName(QStringLiteral("mRoomName"));
    mRoomName->setTextInteractionFlags(Qt::TextBrowserInteraction);
    roomNameLayout->addWidget(mRoomName);
    mRoomName->setVisible(false);

    mTeamName->setObjectName(QStringLiteral("mTeamName"));
    roomNameLayout->addWidget(mTeamName);
    mTeamName->setVisible(false);
    roomNameLayout->addStretch(1);

    mRoomHeaderLabel->setObjectName(QStringLiteral("mRoomHeaderLabel"));
    infoLayout->addWidget(mRoomHeaderLabel);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->setObjectName(QStringLiteral("buttonLayout"));
    buttonLayout->setContentsMargins({});
    buttonLayout->setSpacing(0);
    headerLayout->addLayout(buttonLayout);

    mChannelInfoButton->setAutoRaise(true);
    mChannelInfoButton->setObjectName(QStringLiteral("mChannelInfoButton"));
    mChannelInfoButton->setIcon(QIcon::fromTheme(QStringLiteral("documentinfo")));
    mChannelInfoButton->setToolTip(i18n("Channel's Information"));
    buttonLayout->addWidget(mChannelInfoButton, 0, Qt::AlignTop);
    connect(mChannelInfoButton, &QToolButton::clicked, this, &RoomHeaderWidget::channelInfoRequested);

    mTeamChannelsButton->setAutoRaise(true);
    mTeamChannelsButton->setObjectName(QStringLiteral("mTeamChannelsButton"));
    mTeamChannelsButton->setIcon(QIcon::fromTheme(QStringLiteral("irc-channel-active")));
    mTeamChannelsButton->setToolTip(i18n("Team Channels"));
    buttonLayout->addWidget(mTeamChannelsButton, 0, Qt::AlignTop);
    connect(mTeamChannelsButton, &QToolButton::clicked, this, &RoomHeaderWidget::teamChannelsRequested);

    mListOfUsersButton->setAutoRaise(true);
    mListOfUsersButton->setObjectName(QStringLiteral("mListOfUsersButton"));
    mListOfUsersButton->setIcon(QIcon::fromTheme(QStringLiteral("system-users")));
    mListOfUsersButton->setCheckable(true);
    mListOfUsersButton->setToolTip(i18n("Show List of Users"));
    buttonLayout->addWidget(mListOfUsersButton, 0, Qt::AlignTop);
    connect(mListOfUsersButton, &QToolButton::clicked, this, &RoomHeaderWidget::listOfUsersChanged);

    mSearchMessageButton->setAutoRaise(true);
    mSearchMessageButton->setObjectName(QStringLiteral("mSearchMessageButton"));
    mSearchMessageButton->setIcon(QIcon::fromTheme(QStringLiteral("edit-find")));
    mSearchMessageButton->setCheckable(false);
    mSearchMessageButton->setShortcut(Qt::CTRL + Qt::Key_F);
    mSearchMessageButton->setToolTip(i18n("Search Messages..."));
    buttonLayout->addWidget(mSearchMessageButton, 0, Qt::AlignTop);
    connect(mSearchMessageButton, &QToolButton::clicked, this, &RoomHeaderWidget::searchMessageRequested);

    mChannelAction->setAutoRaise(true);
    mChannelAction->setObjectName(QStringLiteral("mChannelAction"));
    mChannelAction->setPopupMode(QToolButton::InstantPopup);
    mChannelAction->setIcon(QIcon::fromTheme(QStringLiteral("application-menu")));
    buttonLayout->addWidget(mChannelAction, 0, Qt::AlignTop);

    mChannelActionPopupMenu = new ChannelActionPopupMenu(mChannelAction);
    mChannelActionPopupMenu->setObjectName(QStringLiteral("mChannelActionPopupMenu"));
    mChannelAction->setMenu(mChannelActionPopupMenu->menu());
    connect(mChannelActionPopupMenu, &ChannelActionPopupMenu::actionRequested, this, &RoomHeaderWidget::actionRequested);
    connect(mTeamName, &TeamNameLabel::openTeam, this, &RoomHeaderWidget::openTeam);
}

RoomHeaderWidget::~RoomHeaderWidget() = default;

void RoomHeaderWidget::setTeamRoomInfo(const Room::TeamRoomInfo &info)
{
    mTeamName->setTeamRoomInfo(info);
    mTeamName->setVisible(!info.teamName.isEmpty());
}

void RoomHeaderWidget::setRoomName(const QString &name)
{
    mRoomName->setText(name);
    mRoomName->setVisible(!name.isEmpty());
}

QString RoomHeaderWidget::roomName() const
{
    return mRoomName->text();
}

void RoomHeaderWidget::setRoomAnnouncement(const QString &name)
{
    mRoomHeaderLabel->setRoomAnnouncement(name);
}

void RoomHeaderWidget::setRoomTopic(const QString &name)
{
    mRoomHeaderLabel->setRoomTopic(name);
}

void RoomHeaderWidget::setFavoriteStatus(bool b)
{
    mFavoriteButton->setChecked(b);
}

void RoomHeaderWidget::setEncypted(bool b)
{
    mEncryptedButton->setChecked(b);
    mEncryptedButton->setVisible(b);
}

void RoomHeaderWidget::setIsMainTeam(bool isMainTeam)
{
    mTeamChannelsButton->setVisible(isMainTeam);
    mChannelInfoButton->setToolTip(isMainTeam ? i18n("Team's Information") : i18n("Channel's Information"));
}

void RoomHeaderWidget::setIsDiscussion(bool isDiscussion)
{
    mFavoriteButton->setVisible(!isDiscussion);
    mDiscussionBackButton->setVisible(isDiscussion);
}

void RoomHeaderWidget::setRoom(Room *room)
{
    mChannelActionPopupMenu->setRoom(room);
}

void RoomHeaderWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mChannelActionPopupMenu->setCurrentRocketChatAccount(account);
}
