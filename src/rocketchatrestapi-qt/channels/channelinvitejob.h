/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT ChannelInviteJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelInviteJob(QObject *parent = nullptr);
    ~ChannelInviteJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString inviteUserId() const;
    void setInviteUserId(const QString &userId);

    [[nodiscard]] QString inviteUserName() const;
    void setInviteUserName(const QString &userName);

Q_SIGNALS:
    void inviteDone();

private:
    Q_DISABLE_COPY(ChannelInviteJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mInviteUserId;
    QString mInviteUserName;
};
}
