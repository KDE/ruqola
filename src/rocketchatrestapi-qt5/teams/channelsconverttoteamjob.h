/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT ChannelsConvertToTeamJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChannelsConvertToTeamJob(QObject *parent = nullptr);
    ~ChannelsConvertToTeamJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString channelId() const;
    void setChannelId(const QString &channelId);

Q_SIGNALS:
    void channelConvertToTeamDone(const QJsonObject &obj);

protected:
    Q_REQUIRED_RESULT QString generateErrorMessage(const QString &errorStr) const override;

private:
    Q_DISABLE_COPY(ChannelsConvertToTeamJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mChannelId;
};
}
