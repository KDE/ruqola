/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "abstractusercheck.h"

#include "channels/channelinvitejob.h"
#include "channels/channelkickjob.h"
#include "connection.h"
#include "groups/groupsinvitejob.h"
#include "groups/groupskickjob.h"
#include "restapiabstractjob.h"
#include "rocketchataccount.h"
#include "room.h"
#include "users/userinfojob.h"

#include <KLocalizedString>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

QString AbstractUserCheck::targetUserName() const
{
    // A dedicated test account that isn't normally a channel member, so these checks
    // don't disrupt real users or fail because the target is already present.
    return u"monitor"_s;
}

bool AbstractUserCheck::isPrivateRoom() const
{
    Room *room = mAccount ? mAccount->room(mRoomId) : nullptr;
    return room && room->channelType() == Room::RoomType::Private;
}

void AbstractUserCheck::connectFailure(RocketChatRestApi::RestApiAbstractJob *job, const JobCallback &callback)
{
    connect(job, &RocketChatRestApi::RestApiAbstractJob::failed, this, [callback](const QString &serverError, const QString &description) {
        callback(false, serverError.isEmpty() ? description : serverError);
    });
}

void AbstractUserCheck::resolveUserId(const QString &userName, const std::function<void(bool, const QByteArray &)> &callback)
{
    auto job = new RocketChatRestApi::UserInfoJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    RocketChatRestApi::UserBaseJob::UserInfo info;
    info.userIdentifier = userName;
    info.userInfoType = RocketChatRestApi::UserBaseJob::UserInfoType::UserName;
    job->setUserInfo(info);
    connect(job, &RocketChatRestApi::UserInfoJob::userInfoDone, this, [callback](const QJsonObject &obj) {
        const QByteArray userId = obj.value("user"_L1).toObject().value("_id"_L1).toString().toLatin1();
        callback(!userId.isEmpty(), userId);
    });
    connect(job, &RocketChatRestApi::RestApiAbstractJob::failed, this, [callback](const QString &, const QString &) {
        callback(false, {});
    });
    if (!job->start()) {
        callback(false, {});
    }
}

void AbstractUserCheck::invite(const QByteArray &userId, const JobCallback &callback)
{
    RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo roomInfo;
    roomInfo.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::RoomIdentifier;
    roomInfo.identifier = QString::fromLatin1(mRoomId);

    // Both channels.invite and groups.invite require a userId (not a username).
    if (isPrivateRoom()) {
        auto job = new RocketChatRestApi::GroupsInviteJob(this);
        mAccount->restApi()->initializeRestApiJob(job);
        job->setChannelGroupInfo(roomInfo);
        job->setInviteUserId(QString::fromLatin1(userId));
        connect(job, &RocketChatRestApi::GroupsInviteJob::inviteGroupsDone, this, [callback]() {
            callback(true, {});
        });
        connectFailure(job, callback);
        if (!job->start()) {
            callback(false, i18n("Could not start the invite request."));
        }
    } else {
        auto job = new RocketChatRestApi::ChannelInviteJob(this);
        mAccount->restApi()->initializeRestApiJob(job);
        job->setChannelGroupInfo(roomInfo);
        RocketChatRestApi::ChannelInviteJob::ChannelInviteInfo inviteInfo;
        inviteInfo.identifier = QString::fromLatin1(userId);
        inviteInfo.channelGroupInfoType = RocketChatRestApi::ChannelInviteJob::ChannelInviteInfoType::UserId;
        job->setChannelInviteInfo(inviteInfo);
        connect(job, &RocketChatRestApi::ChannelInviteJob::inviteDone, this, [callback]() {
            callback(true, {});
        });
        connectFailure(job, callback);
        if (!job->start()) {
            callback(false, i18n("Could not start the invite request."));
        }
    }
}

void AbstractUserCheck::kick(const QByteArray &userId, const JobCallback &callback)
{
    RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo roomInfo;
    roomInfo.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::RoomIdentifier;
    roomInfo.identifier = QString::fromLatin1(mRoomId);

    if (isPrivateRoom()) {
        auto job = new RocketChatRestApi::GroupsKickJob(this);
        mAccount->restApi()->initializeRestApiJob(job);
        job->setChannelGroupInfo(roomInfo);
        job->setKickUserId(userId);
        connect(job, &RocketChatRestApi::GroupsKickJob::kickUserDone, this, [callback]() {
            callback(true, {});
        });
        connectFailure(job, callback);
        if (!job->start()) {
            callback(false, i18n("Could not start the kick request."));
        }
    } else {
        auto job = new RocketChatRestApi::ChannelKickJob(this);
        mAccount->restApi()->initializeRestApiJob(job);
        job->setChannelGroupInfo(roomInfo);
        job->setKickUserId(userId);
        connect(job, &RocketChatRestApi::ChannelKickJob::kickUserDone, this, [callback]() {
            callback(true, {});
        });
        connectFailure(job, callback);
        if (!job->start()) {
            callback(false, i18n("Could not start the kick request."));
        }
    }
}

#include "moc_abstractusercheck.cpp"
