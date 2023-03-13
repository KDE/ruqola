/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
class QNetworkRequest;
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GetGroupRolesJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit GetGroupRolesJob(QObject *parent = nullptr);
    ~GetGroupRolesJob() override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

Q_SIGNALS:
    void groupRolesDone(const QJsonObject &obj, const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

protected:
    Q_REQUIRED_RESULT QString jobName() const override;

private:
    Q_DISABLE_COPY(GetGroupRolesJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
