/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GroupsInviteJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit GroupsInviteJob(QObject *parent = nullptr);
    ~GroupsInviteJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString inviteUserId() const;
    void setInviteUserId(const QString &userId);

    Q_REQUIRED_RESULT QString inviteUserName() const;
    void setInviteUserName(const QString &inviteUserName);

Q_SIGNALS:
    void inviteGroupsDone();

private:
    Q_DISABLE_COPY(GroupsInviteJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mInviteUserId;
    QString mInviteUserName;
};
}
