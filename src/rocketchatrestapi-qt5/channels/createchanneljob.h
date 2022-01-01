/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "createroominfo.h"
#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT CreateChannelJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit CreateChannelJob(QObject *parent = nullptr);
    ~CreateChannelJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT CreateRoomInfo createChannelInfo() const;
    void setCreateChannelInfo(const CreateRoomInfo &createChannelInfo);

Q_SIGNALS:
    void createChannelDone(const QJsonObject &reply);
    void addJoinCodeToChannel(const QString &channelId, const QString &password);

protected:
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &detail) override;

private:
    Q_DISABLE_COPY(CreateChannelJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    CreateRoomInfo mCreateChannelInfo;
};
}
