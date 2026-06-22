/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "channels/channelinvitejob.h"
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

    [[nodiscard]] QList<RocketChatRestApi::ChannelInviteJob::ChannelInviteInfo> needUnbanUsers() const;
    void setNeedUnbanUsers(const QList<RocketChatRestApi::ChannelInviteJob::ChannelInviteInfo> &newNeedUnbanUsers);
    void addNeedUnbanUsers(const RocketChatRestApi::ChannelInviteJob::ChannelInviteInfo &newNeedUnbanUsers);

private:
    QList<RocketChatRestApi::ChannelInviteJob::ChannelInviteInfo> mNeedUnbanUsers;
    UnbanUsersWidget *const mUnbanUsersWidget;
};
