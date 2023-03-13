/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt5_export.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT GroupOpenJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit GroupOpenJob(QObject *parent = nullptr);
    ~GroupOpenJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

Q_SIGNALS:
    void groupOpenDone(const QJsonObject &obj, const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

private:
    Q_DISABLE_COPY(GroupOpenJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
