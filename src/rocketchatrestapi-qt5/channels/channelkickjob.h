/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelKickJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelKickJob(QObject *parent = nullptr);
    ~ChannelKickJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString kickUserId() const;
    void setKickUserId(const QString &kickUserId);

Q_SIGNALS:
    void kickUserDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(ChannelKickJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mKickUserId;
};
}
