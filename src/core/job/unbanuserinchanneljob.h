/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "adduserinchanneljob.h"
#include "libruqolacore_export.h"
#include <QObject>
class RocketChatAccount;
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

private:
    QList<AddUserInChannelJob::UserInChannelNeedUnBanJobInfo> mNeedUnbanUsers;
    RocketChatAccount *const mRocketChatAccount;
};
