/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt5_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT ChannelJoinJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelJoinJob(QObject *parent = nullptr);
    ~ChannelJoinJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString joinCode() const;
    void setJoinCode(const QString &joinCode);

protected:
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &detail) override;

Q_SIGNALS:
    void setChannelJoinDone(const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void missingChannelPassword(const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void openArchivedRoom(const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

private:
    Q_DISABLE_COPY(ChannelJoinJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mJoinCode;
};
}
