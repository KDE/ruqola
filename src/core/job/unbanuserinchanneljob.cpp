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

bool UnbanUserInChannelJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start UnbanUserInChannelJob";
        deleteLater();
        return false;
    }
    return findUserNames();
}

void UnbanUserInChannelJob::slotRoomsUnbanUserJobDone(RocketChatRestApi::RoomsUnbanUserJob *job)
{
    mRoomsUnbanUserJobList.removeAll(job);
    if (mRoomsUnbanUserJobList.isEmpty()) {
        slotAddUserInRooms();
    }
}

bool UnbanUserInChannelJob::findUserNames()
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
    connect(job, &RocketChatRestApi::RoomsBannedUsersJob::failed, this, [this](const QString &serverErrorStr) {
        qCWarning(RUQOLA_LOG) << "RoomsBannedUsersJob failed:" << serverErrorStr;
        deleteLater();
    });
    if (!job->start()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start RoomsBannedUsersJob job";
        deleteLater();
        return false;
    }
    return true;
}

void UnbanUserInChannelJob::slotRoomsBannedUsersDone(const QJsonObject &obj, [[maybe_unused]] const QByteArray &roomId)
{
    BannedUsers bannedUsers;
    bannedUsers.parseBannedUsers(obj);
    const QList<BannedUser> bannedUsersList = bannedUsers.bannedUsers();
    for (const auto &user : std::as_const(mNeedUnbanUsers)) {
        for (const auto &bannedUser : bannedUsersList) {
            if (bannedUser.identifier() == user.userName) {
                mUsernames.append(bannedUser.userName());
                break;
            }
        }
    }
    if (mUsernames.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "usernames is empty. It's a bug";
        deleteLater();
    } else {
        for (const auto &username : std::as_const(mUsernames)) {
            auto job = new RocketChatRestApi::RoomsUnbanUserJob(this);
            // qDebug() << "username : " << username;
            const RocketChatRestApi::RoomsUnbanUserJob::RoomsUnbanUserInfo info{
                .type = RocketChatRestApi::RoomsUnbanUserJob::IdentifierType::UserName,
                .identifier = username,
                .roomId = mRoomId,
            };
            job->setRoomsUnbanUserInfo(info);
            mRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::RoomsUnbanUserJob::roomsUnbanUserDone, this, [this, job]() {
                slotRoomsUnbanUserJobDone(job);
            });
            // Without this a failed unban would keep its entry in the list forever and block slotAddUserInRooms().
            connect(job, &RocketChatRestApi::RoomsUnbanUserJob::failed, this, [this, job](const QString &serverErrorStr) {
                qCWarning(RUQOLA_LOG) << "RoomsUnbanUserJob failed:" << serverErrorStr;
                slotRoomsUnbanUserJobDone(job);
            });
            if (!job->start()) {
                qCWarning(RUQOLA_LOG) << "Impossible to start RoomsUnbanUserJob";
                job->deleteLater();
            } else {
                mRoomsUnbanUserJobList.append(job);
            }
        }
        if (mRoomsUnbanUserJobList.isEmpty()) {
            qCWarning(RUQOLA_LOG) << "Impossible to start any RoomsUnbanUserJob";
            deleteLater();
        }
    }
}

void UnbanUserInChannelJob::slotAddUserInRooms()
{
    auto addUserInRoomJob = new RocketChatRestApi::MethodCallJob(this);
    QJsonObject obj;
    obj["rid"_L1] = QString::fromLatin1(mRoomId);
    obj["users"_L1] = QJsonArray::fromStringList(mUsernames);
    const QJsonArray params{obj};

    RocketChatRestApi::MethodCallJob::MethodCallJobInfo addUserInRoomInfo;
    addUserInRoomInfo.methodName = u"addUsersToRoom"_s;
    addUserInRoomInfo.anonymous = false;
    addUserInRoomInfo.messageObj = mRocketChatAccount->ddp()->generateJsonObject(addUserInRoomInfo.methodName, params);
    addUserInRoomJob->setMethodCallJobInfo(addUserInRoomInfo);
    mRocketChatAccount->restApi()->initializeRestApiJob(addUserInRoomJob);
    // qDebug()<< " mRestApiConnection " << mRestApiConnection->serverUrl();
    connect(addUserInRoomJob, &RocketChatRestApi::MethodCallJob::methodCallDone, this, [this]() {
        deleteLater();
    });
    // methodCallFailed reports a method which threw server-side, failed() the http/network errors.
    connect(addUserInRoomJob, &RocketChatRestApi::MethodCallJob::methodCallFailed, this, [this]() {
        deleteLater();
    });
    connect(addUserInRoomJob, &RocketChatRestApi::MethodCallJob::failed, this, [this](const QString &serverErrorStr) {
        qCWarning(RUQOLA_LOG) << "addUsersToRoom failed:" << serverErrorStr;
        deleteLater();
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
