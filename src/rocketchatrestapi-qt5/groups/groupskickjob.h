/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GroupsKickJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit GroupsKickJob(QObject *parent = nullptr);
    ~GroupsKickJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString kickUserId() const;
    void setKickUserId(const QString &kickUserId);

Q_SIGNALS:
    void kickUserDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(GroupsKickJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mKickUserId;
};
}
