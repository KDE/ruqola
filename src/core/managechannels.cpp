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

#include "managechannels.h"

#include "channelgroupbasejob.h"
#include "channels/channelopenjob.h"
#include "groups/groupopenjob.h"
#include "restapirequest.h"
#include "ruqola.h"
#include "ruqola_debug.h"

ManageChannels::ManageChannels(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mAccount(account)
{
}

ManageChannels::~ManageChannels()
{
}

bool ManageChannels::searchOpenChannels(const QString &roomId)
{
    bool foundRoom = false;
    bool roomIsOpen = false;
    for (int roomIdx = 0, nRooms = mAccount->roomModel()->rowCount(); roomIdx < nRooms; ++roomIdx) {
        const auto roomModelIndex = mAccount->roomModel()->index(roomIdx, 0);
        const auto identifier = roomModelIndex.data(RoomModel::RoomId).toString();
        if (identifier == roomId) {
            if (roomModelIndex.data(RoomModel::RoomOpen).toBool()) {
                roomIsOpen = true;
                Q_EMIT selectRoomByRoomIdRequested(roomId);
            }
            foundRoom = true;
            break;
        }
    }
    if (roomIsOpen) {
        return true;
    }
    return false;
}

void ManageChannels::openPrivateGroup(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo)
{
    if (searchOpenChannels(roomId)) {
        return;
    }
#if 0

    bool foundRoom = false;
    bool roomIsOpen = false;
    for (int roomIdx = 0, nRooms = mRoomModel->rowCount(); roomIdx < nRooms; ++roomIdx) {
        const auto roomModelIndex = mRoomModel->index(roomIdx, 0);
        const auto identifier = roomModelIndex.data(RoomModel::RoomId).toString();
        if (identifier == roomId) {
            if (roomModelIndex.data(RoomModel::RoomOpen).toBool()) {
                roomIsOpen = true;
                Q_EMIT selectRoomByRoomIdRequested(roomId);
            }
            foundRoom = true;
            break;
        }
    }
    if (roomIsOpen) {
        return;
    }
    RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info;
    switch (typeInfo) {
    case ChannelTypeInfo::RoomId:
        info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
        break;
    case ChannelTypeInfo::RoomName:
        info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Name;
        break;
    }
    info.identifier = roomId;
    if (foundRoom) {
        qCDebug(RUQOLA_LOG) << "opening group" << roomId;
        auto job = new RocketChatRestApi::GroupOpenJob(this);
        job->setChannelGroupInfo(info);
        restApi()->initializeRestApiJob(job);
        connect(job,
                &RocketChatRestApi::GroupOpenJob::groupOpenDone,
                this,
                [this](const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo) {
                    Q_EMIT selectRoomByRoomIdRequested(channelInfo.identifier);
                });
        if (!job->start()) {
            qCWarning(RUQOLA_LOG) << "Impossible to start GroupOpenJob job";
        }
    } else {
        // TODO verify
        restApi()->channelJoin(info, QString());
    }
#endif
}

void ManageChannels::openChannel(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo)
{
    if (searchOpenChannels(roomId)) {
        return;
    }
#if 0
    RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info;
    switch (typeInfo) {
    case ChannelTypeInfo::RoomId:
        info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
        break;
    case ChannelTypeInfo::RoomName:
        info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Name;
        break;
    }
    info.identifier = roomId;
    qCDebug(RUQOLA_LOG) << "opening channel" << roomId;
    if (foundRoom) {
        auto job = new RocketChatRestApi::ChannelOpenJob(this);
        job->setChannelGroupInfo(info);
        restApi()->initializeRestApiJob(job);
        connect(job,
                &RocketChatRestApi::ChannelOpenJob::channelOpenDone,
                this,
                [this](const QJsonObject &obj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo) {
                    Q_EMIT selectRoomByRoomIdRequested(channelInfo.identifier);
                });
        if (!job->start()) {
            qCWarning(RUQOLA_LOG) << "Impossible to start ChannelOpenJob job";
        }
    } else {
        restApi()->channelJoin(info, QString());
    }
#endif
}

void ManageChannels::setChannelJoinDone(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo)
{
#if 0
    ddp()->subscribeRoomMessage(channelInfo.identifier);
    // FIXME room is not added yet...
    switch (channelInfo.channelGroupInfoType) {
    case RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Unknown:
        qCWarning(RUQOLA_LOG) << "setChannelJoinDone : RocketChatRestApi::ChannelBaseJob::ChannelInfoType::Unknown";
        break;
    case RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Identifier:
        Q_EMIT selectRoomByRoomIdRequested(channelInfo.identifier);
        break;
    case RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Name:
        Q_EMIT selectRoomByRoomNameRequested(channelInfo.identifier);
        break;
    }
#endif
}
