/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelAddOwnerJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelAddOwnerJob(QObject *parent = nullptr);
    ~ChannelAddOwnerJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString addownerUserId() const;
    void setAddownerUserId(const QString &addownerUserId);

Q_SIGNALS:
    void addOwnerDone();

private:
    Q_DISABLE_COPY(ChannelAddOwnerJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mAddownerUserId;
};
}
