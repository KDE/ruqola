/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT SetJoinCodeChannelJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit SetJoinCodeChannelJob(QObject *parent = nullptr);
    ~SetJoinCodeChannelJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString joinCode() const;
    void setJoinCode(const QString &joinCode);

Q_SIGNALS:
    void setJoinCodeDone();

private:
    Q_DISABLE_COPY(SetJoinCodeChannelJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mJoinCode;
};
}
