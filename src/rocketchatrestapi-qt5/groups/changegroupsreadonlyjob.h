/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChangeGroupsReadonlyJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChangeGroupsReadonlyJob(QObject *parent = nullptr);
    ~ChangeGroupsReadonlyJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] bool readOnly() const;
    void setReadOnly(bool readOnly);

Q_SIGNALS:
    void changeReadonlyDone();

private:
    Q_DISABLE_COPY(ChangeGroupsReadonlyJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    bool mReadOnly = false;
};
}
