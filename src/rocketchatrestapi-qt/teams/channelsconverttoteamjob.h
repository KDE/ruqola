/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ChannelsConvertToTeamJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChannelsConvertToTeamJob(QObject *parent = nullptr);
    ~ChannelsConvertToTeamJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString channelId() const;
    void setChannelId(const QString &channelId);

Q_SIGNALS:
    void channelConvertToTeamDone(const QJsonObject &obj);

protected:
    [[nodiscard]] QString generateErrorMessage(const QString &errorStr) const override;

private:
    Q_DISABLE_COPY(ChannelsConvertToTeamJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mChannelId;
};
}
