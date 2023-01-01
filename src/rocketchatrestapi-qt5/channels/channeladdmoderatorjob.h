/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelAddModeratorJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelAddModeratorJob(QObject *parent = nullptr);
    ~ChannelAddModeratorJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;
    Q_REQUIRED_RESULT QString addModeratorUserId() const;
    void setAddModeratorUserId(const QString &addModeratorUserId);

Q_SIGNALS:
    void addModeratorDone();

private:
    Q_DISABLE_COPY(ChannelAddModeratorJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mAddModeratorUserId;
};
}
