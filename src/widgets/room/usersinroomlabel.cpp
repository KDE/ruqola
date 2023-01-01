/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomlabel.h"
#include "rocketchataccount.h"
#include "room.h"
#include "roomutil.h"
#include "ruqola.h"
#include "usersinroommenu.h"
#include <QHBoxLayout>
#include <QIcon>

UsersInRoomLabel::UsersInRoomLabel(QWidget *parent)
    : QWidget(parent)
    , mIconLabel(new QLabel(this))
    , mUserNameLabel(new UserLabel(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);
    mIconLabel->setObjectName(QStringLiteral("mIconLabel"));
    mainLayout->addWidget(mIconLabel);

    mUserNameLabel->setObjectName(QStringLiteral("mUserNameLabel"));
    mainLayout->addWidget(mUserNameLabel);
}

UsersInRoomLabel::~UsersInRoomLabel() = default;

void UsersInRoomLabel::setUserInfo(const UsersInRoomLabel::UserInfo &info)
{
    mUserNameLabel->setText(info.userDisplayName);
    mIconLabel->setPixmap(QIcon::fromTheme(info.iconStatus).pixmap(18, 18));
    mUserNameLabel->setUserId(info.userId);
    mUserNameLabel->setUserName(info.userName);
}

void UsersInRoomLabel::setRoom(Room *room)
{
    mUserNameLabel->setRoom(room);
}

UserLabel::UserLabel(QWidget *parent)
    : QLabel(parent)
{
    setTextFormat(Qt::RichText);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &UserLabel::customContextMenuRequested, this, &UserLabel::slotCustomContextMenuRequested);
    connect(this, &QLabel::linkActivated, this, &UserLabel::slotOpenConversation);
}

UserLabel::~UserLabel() = default;

void UserLabel::setRoom(Room *room)
{
    mRoom = room;
}

void UserLabel::setUserName(const QString &userName)
{
    mUserName = userName;
}

void UserLabel::setUserId(const QString &userId)
{
    mUserId = userId;
}

void UserLabel::slotOpenConversation()
{
    Q_EMIT Ruqola::self()->rocketChatAccount()->openLinkRequested(RoomUtil::generateUserLink(mUserName));
}

void UserLabel::slotCustomContextMenuRequested(const QPoint &pos)
{
    if (!mMenu) {
        mMenu = new UsersInRoomMenu(this);
        mMenu->setParentWidget(this);
        mMenu->setRoom(mRoom);
        mMenu->setUserId(mUserId);
        mMenu->setUserName(mUserName);
    }
    mMenu->slotCustomContextMenuRequested(pos);
}
