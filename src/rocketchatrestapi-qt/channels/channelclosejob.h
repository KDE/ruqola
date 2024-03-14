/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ChannelCloseJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    enum ChannelType {
        Channel = 0,
        Groups,
        Direct,
        Unknown,
    };
    Q_ENUM(ChannelType)
    explicit ChannelCloseJob(QObject *parent = nullptr);
    ~ChannelCloseJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] ChannelType channelType() const;
    void setChannelType(RocketChatRestApi::ChannelCloseJob::ChannelType channelType);

Q_SIGNALS:
    void closeChannelDone();

private:
    Q_DISABLE_COPY(ChannelCloseJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    ChannelType mChannelType = Unknown;
};
}
