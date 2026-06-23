/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "unbanuserinchanneljob.h"
#include "bannedusers/bannedusers.h"
#include "connection.h"
#include "misc/methodcalljob.h"
#include "rocketchataccount.h"
#include "rooms/roomsbannedusersjob.h"
#include "rooms/roomsunbanuserjob.h"
#include "ruqola_debug.h"
#include <QJsonArray>

UnbanUserInChannelJob::UnbanUserInChannelJob(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mRocketChatAccount(account)
{
}

UnbanUserInChannelJob::~UnbanUserInChannelJob() = default;

QList<AddUserInChannelJob::UserInChannelNeedUnBanJobInfo> UnbanUserInChannelJob::needUnbanUsers() const
{
    return mNeedUnbanUsers;
}

void UnbanUserInChannelJob::setNeedUnbanUsers(const QList<AddUserInChannelJob::UserInChannelNeedUnBanJobInfo> &newNeedUnbanUsers)
{
    mNeedUnbanUsers = newNeedUnbanUsers;
}

bool UnbanUserInChannelJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start UnbanUserInChannelJob! mRoomId is empty";
        return false;
    }
    return !mNeedUnbanUsers.isEmpty();
}

void UnbanUserInChannelJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start UnbanUserInChannelJob";
        deleteLater();
        return;
    }

    for (const auto &user : std::as_const(mNeedUnbanUsers)) {
        auto job = new RocketChatRestApi::RoomsUnbanUserJob(this);
        qDebug() << "user : " << user;
        const RocketChatRestApi::RoomsUnbanUserJob::RoomsUnbanUserInfo info{
            .type = RocketChatRestApi::RoomsUnbanUserJob::IdentifierType::UserId,
            .identifier = user.userName,
            .roomId = user.roomId,
        };
        job->setRoomsUnbanUserInfo(info);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::RoomsUnbanUserJob::roomsUnbanUserDone, this, [this, job]() {
            slotRoomsUnbanUserJobDone(job);
        });
        if (!job->start()) {
            qCWarning(RUQOLA_LOG) << "Impossible to start RoomsUnbanUserJob";
            job->deleteLater();
        } else {
            mRoomsUnbanUserJobList.append(job);
        }
    }
}

void UnbanUserInChannelJob::slotRoomsUnbanUserJobDone(RocketChatRestApi::RoomsUnbanUserJob *job)
{
    mRoomsUnbanUserJobList.removeAll(job);
    if (mRoomsUnbanUserJobList.isEmpty()) {
        findUserNames();
    }
}

void UnbanUserInChannelJob::findUserNames()
{
    auto job = new RocketChatRestApi::RoomsBannedUsersJob(this);
    const RocketChatRestApi::RoomsBannedUsersJob::RoomsBannedUsersJobInfo info{
        .roomId = mRoomId,
        .offset = 0,
        .count = 50,
    };
    job->setRoomsBannedUsersJobInfo(info);

    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::RoomsBannedUsersJob::roomsBannedUsersDone, this, &UnbanUserInChannelJob::slotRoomsBannedUsersDone);
    if (!job->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start RoomsBannedUsersJob job";
        deleteLater();
    }
}

void UnbanUserInChannelJob::slotRoomsBannedUsersDone(const QJsonObject &obj, [[maybe_unused]] const QByteArray &roomId)
{
    BannedUsers bannedUsers;
    bannedUsers.parseBannedUsers(obj);
    const QList<BannedUser> bannedUsersList = bannedUsers.bannedUsers();
    QStringList usernames;
    for (const auto &user : std::as_const(mNeedUnbanUsers)) {
        for (const auto &bannedUser : bannedUsersList) {
            if (bannedUser.identifier() == user.userName) {
                usernames.append(bannedUser.userName());
                break;
            }
        }
    }
    if (usernames.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "usernames is empty. It's a bug";
        deleteLater();
    } else {
        slotAddUserInRooms(usernames);
    }
}

void UnbanUserInChannelJob::slotAddUserInRooms(const QStringList &usernames)
{
    auto addUserInRoomJob = new RocketChatRestApi::MethodCallJob(this);
    QJsonObject obj;
    obj["rid"_L1] = QString::fromLatin1(mRoomId);
    obj["users"_L1] = QJsonArray::fromStringList(usernames);
    const QJsonArray params{obj};
    qDebug() << " CCCCCCCCCC  " << params;

    RocketChatRestApi::MethodCallJob::MethodCallJobInfo addUserInRoomInfo;
    addUserInRoomInfo.methodName = u"addUsersToRoom"_s;
    addUserInRoomInfo.anonymous = false;
    addUserInRoomInfo.messageObj = mRocketChatAccount->ddp()->generateJsonObject(addUserInRoomInfo.methodName, params);
    addUserInRoomJob->setMethodCallJobInfo(addUserInRoomInfo);
    mRocketChatAccount->restApi()->initializeRestApiJob(addUserInRoomJob);
    // qDebug()<< " mRestApiConnection " << mRestApiConnection->serverUrl();
    connect(addUserInRoomJob, &RocketChatRestApi::MethodCallJob::methodCallDone, this, [this](const QJsonObject &root) {
        const QJsonObject obj = root.value("result"_L1).toObject();
        deleteLater();
        // qCDebug(RUQOLA_DDPAPI_LOG) << obj.value("messages")).toArray().size();
    });
    if (!addUserInRoomJob->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start addUsersToRoom job";
        deleteLater();
    }
}

QByteArray UnbanUserInChannelJob::roomId() const
{
    return mRoomId;
}

void UnbanUserInChannelJob::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

#include "moc_unbanuserinchanneljob.cpp"
