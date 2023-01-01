/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelRemoveLeaderJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelRemoveLeaderJob(QObject *parent = nullptr);
    ~ChannelRemoveLeaderJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString removeUserId() const;
    void setRemoveUserId(const QString &removeUserId);

Q_SIGNALS:
    void removeLeaderDone();

private:
    Q_DISABLE_COPY(ChannelRemoveLeaderJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mRemoveUserId;
};
}
