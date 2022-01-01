/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt5_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT GroupsDeleteJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit GroupsDeleteJob(QObject *parent = nullptr);
    ~GroupsDeleteJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;
Q_SIGNALS:
    void deleteGroupsDone(const QString &identifier);

private:
    Q_DISABLE_COPY(GroupsDeleteJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
};
}
