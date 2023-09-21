/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "rocketchataccount.h"
#include <QObject>
class LIBRUQOLACORE_TESTS_EXPORT ManageChannels : public QObject
{
    Q_OBJECT
public:
    enum SearchChannelFound {
        NoFound = 0,
        ChannelOpened,
        ChannelHidden,
    };
    explicit ManageChannels(RocketChatAccount *account, QObject *parent = nullptr);
    ~ManageChannels() override;

    void openPrivateGroup(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo);
    void openChannel(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo);

    void channelJoin(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo, const QString &joinCode);
Q_SIGNALS:
    // TODO ?
    void selectRoomByRoomIdRequested(const QString &identifier, const QString &messageId = QString());
    void selectRoomByRoomNameRequested(const QString &identifier);
    void missingChannelPassword(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void openArchivedRoom(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

private:
    void setChannelJoinDone(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    [[nodiscard]] SearchChannelFound searchOpenChannels(const QString &roomId);
    [[nodiscard]] RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo generateGroupInfo(const QString &roomId,
                                                                                             RocketChatAccount::ChannelTypeInfo typeInfo);
    RocketChatAccount *const mAccount;
};
