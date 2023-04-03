/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt5_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT ChannelDeleteJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelDeleteJob(QObject *parent = nullptr);
    ~ChannelDeleteJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

Q_SIGNALS:
    void deletechannelDone(const QString &identifier);

private:
    Q_DISABLE_COPY(ChannelDeleteJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
