/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelAddLeaderJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelAddLeaderJob(QObject *parent = nullptr);
    ~ChannelAddLeaderJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString addLeaderUserId() const;
    void setAddLeaderUserId(const QString &addLeaderUserId);

Q_SIGNALS:
    void addLeaderDone();

private:
    Q_DISABLE_COPY(ChannelAddLeaderJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mAddLeaderUserId;
};
}
