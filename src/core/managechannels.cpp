/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managechannels.h"

#include "channels/channeljoinjob.h"
#include "channels/channelopenjob.h"
#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "groups/groupopenjob.h"
#include "model/roommodel.h"
#include "ruqola.h"
#include "ruqola_channel_management_debug.h"

#include <QTimer>

ManageChannels::ManageChannels(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mAccount(account)
{
}

ManageChannels::~ManageChannels() = default;

// TODO port to QByteArray ?
ManageChannels::SearchChannelFound ManageChannels::searchOpenChannels(const QString &roomId)
{
    ManageChannels::SearchChannelFound result = ManageChannels::SearchChannelFound::NoFound;
    const auto roomModel = mAccount->roomModel();
    for (int roomIdx = 0, nRooms = roomModel->rowCount(); roomIdx < nRooms; ++roomIdx) {
        const auto roomModelIndex = roomModel->index(roomIdx, 0);
        const auto identifier = roomModelIndex.data(RoomModel::RoomId).toByteArray();
        if (QString::fromLatin1(identifier) == roomId) {
            if (roomModelIndex.data(RoomModel::RoomOpen).toBool()) {
                result = ManageChannels::SearchChannelFound::ChannelOpened;
                Q_EMIT selectRoomByRoomIdRequested(roomId.toLatin1());
            } else {
                result = ManageChannels::SearchChannelFound::ChannelHidden;
            }
            break;
        }
    }
    return result;
}

RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo ManageChannels::generateGroupInfo(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo)
{
    RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info;
    switch (typeInfo) {
    case RocketChatAccount::ChannelTypeInfo::RoomId:
        info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
        break;
    case RocketChatAccount::ChannelTypeInfo::RoomName:
        info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Name;
        break;
    }
    info.identifier = roomId;
    return info;
}

void ManageChannels::channelJoin(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo, const QString &joinCode)
{
    auto job = new RocketChatRestApi::ChannelJoinJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    job->setJoinCode(joinCode);
    job->setChannelGroupInfo(channelInfo);
    connect(job, &RocketChatRestApi::ChannelJoinJob::setChannelJoinDone, this, &ManageChannels::setChannelJoinDone);
    connect(job, &RocketChatRestApi::ChannelJoinJob::missingChannelPassword, this, &ManageChannels::missingChannelPassword);
    connect(job, &RocketChatRestApi::ChannelJoinJob::openArchivedRoom, this, &ManageChannels::openArchivedRoom);
    if (!job->start()) {
        qCDebug(RUQOLA_CHANNEL_MANAGEMENT_LOG) << "Impossible to start setChannelJoin";
    }
}

void ManageChannels::verifyNeedSelectChannel(const QByteArray &rid)
{
    if (mDelayedSelectedRooms.contains(rid)) {
        // Need to delay it until model was updated
        QTimer::singleShot(0, this, [this, rid]() {
            Q_EMIT selectRoomByRoomIdRequested(rid);
        });

        mDelayedSelectedRooms.removeAll(rid);
    }
}

void ManageChannels::delaySelectChannelRequested(const QByteArray &rid)
{
    if (!mDelayedSelectedRooms.contains(rid)) {
        mDelayedSelectedRooms.append(rid);
    }
}

void ManageChannels::openPrivateGroup(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo)
{
    const ManageChannels::SearchChannelFound result = searchOpenChannels(roomId);
    if (result == ManageChannels::SearchChannelFound::ChannelOpened) {
        return;
    }

    const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info = generateGroupInfo(roomId, typeInfo);
    qCDebug(RUQOLA_CHANNEL_MANAGEMENT_LOG) << "opening group" << roomId;
    switch (result) {
    case ManageChannels::SearchChannelFound::ChannelOpened:
        // Nothing
        break;
    case ManageChannels::SearchChannelFound::NoFound: {
        // TODO verify
        channelJoin(info, QString());
        break;
    }
    case ManageChannels::SearchChannelFound::ChannelHidden: {
        auto job = new RocketChatRestApi::GroupOpenJob(this);
        job->setChannelGroupInfo(info);
        mAccount->restApi()->initializeRestApiJob(job);
        connect(job,
                &RocketChatRestApi::GroupOpenJob::groupOpenDone,
                this,
                [this](const QJsonObject &, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo) {
                    Q_EMIT selectRoomByRoomIdRequested(channelInfo.identifier.toLatin1());
                });
        if (!job->start()) {
            qCWarning(RUQOLA_CHANNEL_MANAGEMENT_LOG) << "Impossible to start GroupOpenJob job";
        }
        break;
    }
    }
}

void ManageChannels::openChannel(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo)
{
    const ManageChannels::SearchChannelFound result = searchOpenChannels(roomId);
    if (result == ManageChannels::SearchChannelFound::ChannelOpened) {
        return;
    }
    const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info = generateGroupInfo(roomId, typeInfo);
    qCDebug(RUQOLA_CHANNEL_MANAGEMENT_LOG) << "opening channel" << roomId;
    switch (result) {
    case ManageChannels::SearchChannelFound::ChannelOpened:
        // Nothing
        break;
    case ManageChannels::SearchChannelFound::NoFound: {
        channelJoin(info, QString());
        break;
    }
    case ManageChannels::SearchChannelFound::ChannelHidden: {
        auto job = new RocketChatRestApi::ChannelOpenJob(this);
        job->setChannelGroupInfo(info);
        mAccount->restApi()->initializeRestApiJob(job);
        connect(job,
                &RocketChatRestApi::ChannelOpenJob::channelOpenDone,
                this,
                [this](const QJsonObject &, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo) {
                    Q_EMIT selectRoomByRoomIdRequested(channelInfo.identifier.toLatin1());
                });
        if (!job->start()) {
            qCWarning(RUQOLA_CHANNEL_MANAGEMENT_LOG) << "Impossible to start ChannelOpenJob job";
        }
        break;
    }
    }
}

void ManageChannels::setChannelJoinDone(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo)
{
    qCDebug(RUQOLA_CHANNEL_MANAGEMENT_LOG) << " setChannelJoinDone type:" << channelInfo.channelGroupInfoType << " channelInfo.identifier "
                                           << channelInfo.identifier;
    mAccount->ddp()->subscribeRoomMessage(channelInfo.identifier.toLatin1());
    // FIXME room is not added yet...
    switch (channelInfo.channelGroupInfoType) {
    case RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Unknown:
        qCWarning(RUQOLA_CHANNEL_MANAGEMENT_LOG) << "setChannelJoinDone : RocketChatRestApi::ChannelBaseJob::ChannelInfoType::Unknown";
        break;
    case RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Identifier:
        Q_EMIT selectRoomByRoomIdRequested(channelInfo.identifier.toLatin1());
        break;
    case RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Name:
        Q_EMIT selectRoomByRoomNameRequested(channelInfo.identifier);
        break;
    }
}

#include "moc_managechannels.cpp"
