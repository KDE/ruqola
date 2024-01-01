/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT GroupsInviteJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit GroupsInviteJob(QObject *parent = nullptr);
    ~GroupsInviteJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString inviteUserId() const;
    void setInviteUserId(const QString &userId);

    [[nodiscard]] QString inviteUserName() const;
    void setInviteUserName(const QString &inviteUserName);

Q_SIGNALS:
    void inviteGroupsDone();

private:
    Q_DISABLE_COPY(GroupsInviteJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mInviteUserId;
    QString mInviteUserName;
};
}
