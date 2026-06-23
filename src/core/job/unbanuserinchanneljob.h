/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "adduserinchanneljob.h"
#include "libruqolacore_export.h"
#include <QObject>
class RocketChatAccount;
namespace RocketChatRestApi
{
class RoomsUnbanUserJob;
};
class LIBRUQOLACORE_EXPORT UnbanUserInChannelJob : public QObject
{
    Q_OBJECT
public:
    explicit UnbanUserInChannelJob(RocketChatAccount *account, QObject *parent = nullptr);
    ~UnbanUserInChannelJob() override;

    [[nodiscard]] QList<AddUserInChannelJob::UserInChannelNeedUnBanJobInfo> needUnbanUsers() const;
    void setNeedUnbanUsers(const QList<AddUserInChannelJob::UserInChannelNeedUnBanJobInfo> &newNeedUnbanUsers);

    void start();

    [[nodiscard]] bool canStart() const;

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &newRoomId);

private:
    LIBRUQOLACORE_NO_EXPORT void slotAddUserInRooms(const QStringList &usernames);
    LIBRUQOLACORE_NO_EXPORT void findUserNames();
    LIBRUQOLACORE_NO_EXPORT void slotRoomsBannedUsersDone(const QJsonObject &obj, const QByteArray &roomId);
    LIBRUQOLACORE_NO_EXPORT void slotRoomsUnbanUserJobDone(RocketChatRestApi::RoomsUnbanUserJob *job);
    QList<AddUserInChannelJob::UserInChannelNeedUnBanJobInfo> mNeedUnbanUsers;
    QByteArray mRoomId;
    RocketChatAccount *const mRocketChatAccount;
    QList<RocketChatRestApi::RoomsUnbanUserJob *> mRoomsUnbanUserJobList;
};
