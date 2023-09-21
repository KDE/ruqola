/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt5_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT ChannelDeleteJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelDeleteJob(QObject *parent = nullptr);
    ~ChannelDeleteJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

Q_SIGNALS:
    void deletechannelDone(const QString &identifier);

private:
    Q_DISABLE_COPY(ChannelDeleteJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
