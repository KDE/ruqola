/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChangeGroupsNameJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChangeGroupsNameJob(QObject *parent = nullptr);
    ~ChangeGroupsNameJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &topic);
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

Q_SIGNALS:
    void changeGroupsnameDone();

private:
    Q_DISABLE_COPY(ChangeGroupsNameJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mName;
};
}
