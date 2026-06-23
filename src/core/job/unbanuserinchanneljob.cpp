/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "unbanuserinchanneljob.h"
#include "connection.h"
#include "misc/methodcalljob.h"
#include "rocketchataccount.h"
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
        connect(job, &RocketChatRestApi::RoomsUnbanUserJob::roomsUnbanUserDone, this, [this, user]() {
            auto addUserInRoomJob = new RocketChatRestApi::MethodCallJob(this);
            QJsonObject obj;
            obj["rid"_L1] = QString::fromLatin1(user.roomId);
            obj["users"_L1] = QJsonArray::fromStringList({user.userName});
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
                // qCDebug(RUQOLA_DDPAPI_LOG) << obj.value("messages")).toArray().size();
            });
            if (!addUserInRoomJob->start()) {
                qCWarning(RUQOLA_LOG) << "Impossible to start addUsersToRoom job";
            }
        });
        if (!job->start()) {
            qCWarning(RUQOLA_LOG) << "Impossible to start RoomsUnbanUserJob";
        }
    }
}
