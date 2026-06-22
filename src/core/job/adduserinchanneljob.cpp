/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adduserinchanneljob.h"

#include "connection.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;

AddUserInChannelJob::AddUserInChannelJob(Connection *restApiConnection, QObject *parent)
    : QObject{parent}
    , mConnection(restApiConnection)
{
}

AddUserInChannelJob::~AddUserInChannelJob() = default;

void AddUserInChannelJob::start()
{
    auto job = new ChannelInviteJob(this);
    mConnection->initializeRestApiJob(job);
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::RoomIdentifier;
    info.identifier = QString::fromLatin1(mInfo.roomId);
    job->setChannelGroupInfo(info);
    const ChannelInviteJob::ChannelInviteInfo inviteInfo{
        .identifier = QString::fromLatin1(mInfo.userId),
        .channelGroupInfoType = ChannelInviteJob::ChannelInviteInfoType::UserId,
    };
    job->setChannelInviteInfo(inviteInfo);
    connect(job, &ChannelInviteJob::needUnbanned, this, &AddUserInChannelJob::slotNeedUnbanned);
    connect(job, &ChannelInviteJob::inviteDone, this, &AddUserInChannelJob::deleteLater);
    if (!job->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start addUserInChannel job";
        deleteLater();
    }
}

void AddUserInChannelJob::slotNeedUnbanned(const RocketChatRestApi::ChannelInviteJob::ChannelInviteInfo &info)
{
    Q_EMIT userNeedUnbanned(info);
    deleteLater();
}

AddUserInChannelJob::AddUserInChannelJobInfo AddUserInChannelJob::info() const
{
    return mInfo;
}

void AddUserInChannelJob::setInfo(const AddUserInChannelJobInfo &newInfo)
{
    mInfo = newInfo;
}

bool AddUserInChannelJob::AddUserInChannelJobInfo::isValid() const
{
    return !roomId.isEmpty() && !userId.isEmpty();
}

bool AddUserInChannelJob::canStart() const
{
    return mInfo.isValid();
}

#include "moc_adduserinchanneljob.cpp"
