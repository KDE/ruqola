/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ChannelJoinJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelJoinJob(QObject *parent = nullptr);
    ~ChannelJoinJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString joinCode() const;
    void setJoinCode(const QString &joinCode);

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &detail) override;

Q_SIGNALS:
    void setChannelJoinDone(const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void missingChannelPassword(const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void openArchivedRoom(const ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);

private:
    Q_DISABLE_COPY(ChannelJoinJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mJoinCode;
};
}
