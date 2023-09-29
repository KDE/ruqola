/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "createchannelteaminfo.h"
#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT TeamsCreateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TeamsCreateJob(QObject *parent = nullptr);
    ~TeamsCreateJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] CreateChannelTeamInfo teamsCreateJobInfo() const;
    void setTeamsCreateJobInfo(const CreateChannelTeamInfo &teamsCreateJobInfo);

Q_SIGNALS:
    void teamCreateDone();
    // TODO verify it.
    void addJoinCodeToChannel(const QString &channelId, const QString &password);

private:
    Q_DISABLE_COPY(TeamsCreateJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    CreateChannelTeamInfo mTeamsCreateJobInfo;
};
}
