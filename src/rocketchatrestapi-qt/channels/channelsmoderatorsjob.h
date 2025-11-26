/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ChannelsModeratorsJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelsModeratorsJob(QObject *parent = nullptr);
    ~ChannelsModeratorsJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

Q_SIGNALS:
    void channelFilesDone(const QJsonObject &replyObjectj, const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
