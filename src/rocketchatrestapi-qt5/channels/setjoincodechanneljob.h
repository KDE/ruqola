/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT SetJoinCodeChannelJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit SetJoinCodeChannelJob(QObject *parent = nullptr);
    ~SetJoinCodeChannelJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString joinCode() const;
    void setJoinCode(const QString &joinCode);

Q_SIGNALS:
    void setJoinCodeDone();

private:
    Q_DISABLE_COPY(SetJoinCodeChannelJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mJoinCode;
};
}
