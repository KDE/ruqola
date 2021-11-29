/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GroupRemoveOwnerJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit GroupRemoveOwnerJob(QObject *parent = nullptr);
    ~GroupRemoveOwnerJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString removeUserId() const;
    void setRemoveUserId(const QString &removeUserId);

Q_SIGNALS:
    void groupRemoveOwnerDone();

private:
    Q_DISABLE_COPY(GroupRemoveOwnerJob)
    void slotRemoveOwnerFinished();
    QString mRemoveUserId;
};
}
