/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include "createchannelteaminfo.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT CreateGroupsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit CreateGroupsJob(QObject *parent = nullptr);
    ~CreateGroupsJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] CreateChannelTeamInfo createGroupsInfo() const;
    void setCreateGroupsInfo(const CreateChannelTeamInfo &createGroupsInfo);

Q_SIGNALS:
    void createGroupsDone(const QJsonObject &reply);

private:
    Q_DISABLE_COPY(CreateGroupsJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    CreateChannelTeamInfo mCreateGroupInfo;
    // {"name":"teams-test-room2","members":[],"readOnly":false,"extraData":{"description":"","broadcast":false,"encrypted":false,"teamId":"607fb34ee8da65ad4f476487"}
    // TODO add extraData
};
}
