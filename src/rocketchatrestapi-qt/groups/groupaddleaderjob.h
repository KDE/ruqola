/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT GroupAddLeaderJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit GroupAddLeaderJob(QObject *parent = nullptr);
    ~GroupAddLeaderJob() override;

    bool start() override;
    bool requireHttpAuthentication() const override;
    bool canStart() const override;

    QNetworkRequest request() const override;

    QJsonDocument json() const;

    QString addLeaderUserId() const;
    void setAddLeaderUserId(const QString &addLeaderUserId);

Q_SIGNALS:
    void addLeaderDone();

private:
    Q_DISABLE_COPY(GroupAddLeaderJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mAddLeaderUserId;
};
}
