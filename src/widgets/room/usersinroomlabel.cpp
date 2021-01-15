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

#include "usersinroomlabel.h"
#include "usersinroommenu.h"
#include "room.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "roomutil.h"
#include "dialogs/directchannelinfodialog.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QIcon>
#include <QMenu>
#include <QPointer>
#include <KMessageBox>

UsersInRoomLabel::UsersInRoomLabel(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);
    mIconLabel = new QLabel(this);
    mIconLabel->setObjectName(QStringLiteral("mIconLabel"));
    mainLayout->addWidget(mIconLabel);

    mUserNameLabel = new UserLabel(this);
    mUserNameLabel->setObjectName(QStringLiteral("mUserNameLabel"));
    mainLayout->addWidget(mUserNameLabel);
}

UsersInRoomLabel::~UsersInRoomLabel()
{
}

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

UserLabel::~UserLabel()
{
}

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
