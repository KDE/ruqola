/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT ChangeGroupsNameJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChangeGroupsNameJob(QObject *parent = nullptr);
    ~ChangeGroupsNameJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QString name() const;
    void setName(const QString &topic);
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

Q_SIGNALS:
    void changeGroupsnameDone();

private:
    Q_DISABLE_COPY(ChangeGroupsNameJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mName;
};
}
