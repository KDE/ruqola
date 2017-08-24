/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#include "rocketchataccount.h"
#include "roommodel.h"
#include "typingnotification.h"
#include "usermodel.h"

RocketChatAccount::RocketChatAccount(QObject *parent)
    : QObject(parent)
{
    mRoomModel = new RoomModel(this);
    mUserModel = new UsersModel(this);
    mTypingNotification = new TypingNotification(this);
    connect(mTypingNotification, &TypingNotification::informTypingStatus, this, &RocketChatAccount::slotInformTypingStatus);
    loadSettings();
}

RocketChatAccount::~RocketChatAccount()
{

}

void RocketChatAccount::loadSettings()
{
    mSettings.loadSettings();
}

RocketChatAccountSettings RocketChatAccount::settings() const
{
    return mSettings;
}

void RocketChatAccount::setSettings(const RocketChatAccountSettings &settings)
{
    mSettings = settings;
}

void RocketChatAccount::slotInformTypingStatus(const QString &room, bool typing)
{
    //TODO ddp()->informTypingStatus(room, typing, mUserName);
}

RoomModel *RocketChatAccount::roomModel() const
{
    return mRoomModel;
}

UsersModel *RocketChatAccount::userModel() const
{
    return mUserModel;
}
