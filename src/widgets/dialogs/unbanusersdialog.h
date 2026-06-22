/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "job/adduserinchanneljob.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class UnbanUsersWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UnbanUsersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UnbanUsersDialog(QWidget *parent = nullptr);
    ~UnbanUsersDialog() override;

    void addBanUser(const RocketChatRestApi::ChannelInviteJob::ChannelInviteInfo &info);

    [[nodiscard]] QList<AddUserInChannelJob::UserInChannelNeedUnBanJobInfo> needUnbanUsers() const;
    void setNeedUnbanUsers(const QList<AddUserInChannelJob::UserInChannelNeedUnBanJobInfo> &newNeedUnbanUsers);
    void addNeedUnbanUsers(const AddUserInChannelJob::UserInChannelNeedUnBanJobInfo &newNeedUnbanUsers);

private:
    QList<AddUserInChannelJob::UserInChannelNeedUnBanJobInfo> mNeedUnbanUsers;
    UnbanUsersWidget *const mUnbanUsersWidget;
};
