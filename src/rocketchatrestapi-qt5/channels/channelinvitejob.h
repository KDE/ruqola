/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelInviteJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelInviteJob(QObject *parent = nullptr);
    ~ChannelInviteJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString inviteUserId() const;
    void setInviteUserId(const QString &userId);

    Q_REQUIRED_RESULT QString inviteUserName() const;
    void setInviteUserName(const QString &userName);

Q_SIGNALS:
    void inviteDone();

private:
    Q_DISABLE_COPY(ChannelInviteJob)
    void slotInvitationFinished();
    QString mInviteUserId;
    QString mInviteUserName;
};
}

