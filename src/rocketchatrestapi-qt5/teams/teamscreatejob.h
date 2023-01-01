/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "createchannelteaminfo.h"
#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT TeamsCreateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamsCreateJob(QObject *parent = nullptr);
    ~TeamsCreateJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT CreateChannelTeamInfo teamsCreateJobInfo() const;
    void setTeamsCreateJobInfo(const CreateChannelTeamInfo &teamsCreateJobInfo);

Q_SIGNALS:
    void teamCreateDone();
    // TODO verify it.
    void addJoinCodeToChannel(const QString &channelId, const QString &password);

private:
    Q_DISABLE_COPY(TeamsCreateJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    CreateChannelTeamInfo mTeamsCreateJobInfo;
};
}
