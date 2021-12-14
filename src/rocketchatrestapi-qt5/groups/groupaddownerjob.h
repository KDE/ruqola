/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GroupAddOwnerJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit GroupAddOwnerJob(QObject *parent = nullptr);
    ~GroupAddOwnerJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString addownerUserId() const;
    void setAddownerUserId(const QString &addownerUserId);

Q_SIGNALS:
    void addOwnerDone();

private:
    Q_DISABLE_COPY(GroupAddOwnerJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mAddownerUserId;
};
}
