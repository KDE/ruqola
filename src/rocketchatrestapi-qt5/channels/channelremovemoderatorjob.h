/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelRemoveModeratorJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChannelRemoveModeratorJob(QObject *parent = nullptr);
    ~ChannelRemoveModeratorJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString removeUserId() const;
    void setRemoveUserId(const QString &removeUserId);

Q_SIGNALS:
    void removeModeratorDone();

private:
    Q_DISABLE_COPY(ChannelRemoveModeratorJob)
    void slotRemoveModeratorFinished();
    QString mRemoveUserId;
};
}
