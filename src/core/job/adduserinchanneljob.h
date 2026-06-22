/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "channels/channelinvitejob.h"
#include "libruqolacore_export.h"
#include <QObject>
class Connection;
class LIBRUQOLACORE_EXPORT AddUserInChannelJob : public QObject
{
    Q_OBJECT
public:
    struct AddUserInChannelJobInfo {
        QByteArray roomId;
        QByteArray userId;
        [[nodiscard]] bool isValid() const;
    };

    explicit AddUserInChannelJob(Connection *restApiConnection, QObject *parent = nullptr);
    ~AddUserInChannelJob() override;

    [[nodiscard]] bool canStart() const;

    void start();

    [[nodiscard]] AddUserInChannelJobInfo info() const;
    void setInfo(const AddUserInChannelJobInfo &newInfo);

Q_SIGNALS:
    void userNeedUnbanned(const RocketChatRestApi::ChannelInviteJob::ChannelInviteInfo &info);

private:
    LIBRUQOLACORE_NO_EXPORT void slotNeedUnbanned(const RocketChatRestApi::ChannelInviteJob::ChannelInviteInfo &info);
    AddUserInChannelJobInfo mInfo;
    Connection *const mConnection;
};
