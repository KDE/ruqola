/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

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
    enum class SearchChannelFound : uint8_t {
        NoFound = 0,
        ChannelOpened,
        ChannelHidden,
    };
    Q_ENUM(SearchChannelFound);
    explicit ManageChannels(RocketChatAccount *account, QObject *parent = nullptr);
    ~ManageChannels() override;

    void openPrivateGroup(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo);
    void openChannel(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo);

    void channelJoin(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo, const QString &joinCode);
    void delaySelectChannelRequested(const QByteArray &rid);
    void verifyNeedSelectChannel(const QByteArray &rid);
Q_SIGNALS:
    // TODO ?
    void selectRoomByRoomIdRequested(const QByteArray &identifier, const QByteArray &messageId = QByteArray());
    void selectRoomByRoomNameRequested(const QString &identifier);
    void missingChannelPassword(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void openArchivedRoom(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

private:
    LIBRUQOLACORE_NO_EXPORT void setChannelJoinDone(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT SearchChannelFound searchOpenChannels(const QString &roomId);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo
    generateGroupInfo(const QString &roomId, RocketChatAccount::ChannelTypeInfo typeInfo);
    RocketChatAccount *const mAccount;
    QList<QByteArray> mDelayedSelectedRooms;
};
