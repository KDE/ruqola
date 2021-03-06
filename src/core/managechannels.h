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

#pragma once

#include "libruqola_private_export.h"
#include "rocketchataccount.h"
#include <QObject>
class LIBRUQOLACORE_TESTS_EXPORT ManageChannels : public QObject
{
    Q_OBJECT
public:
    enum SearchChannelFound {
        NoFound = 0,
        ChannelOpened,
        ChannelHidden,
    };
    explicit ManageChannels(RocketChatAccount *account, QObject *parent = nullptr);
    ~ManageChannels() override;

    void openPrivateGroup(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo);
    void openChannel(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo);

    void channelJoin(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo, const QString &joinCode);
Q_SIGNALS:
    void selectRoomByRoomIdRequested(const QString &identifier);
    void selectRoomByRoomNameRequested(const QString &identifier);
    void missingChannelPassword(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void openArchivedRoom(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

private:
    void setChannelJoinDone(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    Q_REQUIRED_RESULT SearchChannelFound searchOpenChannels(const QString &roomId);
    Q_REQUIRED_RESULT RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo generateGroupInfo(const QString &roomId,
                                                                                                 RocketChatAccount::ChannelTypeInfo typeInfo);
    RocketChatAccount *const mAccount;
};
