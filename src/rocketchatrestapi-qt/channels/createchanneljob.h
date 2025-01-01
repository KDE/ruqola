/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "createchannelteaminfo.h"
#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT CreateChannelJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit CreateChannelJob(QObject *parent = nullptr);
    ~CreateChannelJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] CreateChannelTeamInfo createChannelInfo() const;
    void setCreateChannelInfo(const CreateChannelTeamInfo &createChannelInfo);

Q_SIGNALS:
    void createChannelDone(const QJsonObject &reply);
    void addJoinCodeToChannel(const QString &channelId, const QString &password);

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &detail) override;

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    CreateChannelTeamInfo mCreateChannelInfo;
};
}
