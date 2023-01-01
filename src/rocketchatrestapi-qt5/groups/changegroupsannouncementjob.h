/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChangeGroupsAnnouncementJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit ChangeGroupsAnnouncementJob(QObject *parent = nullptr);
    ~ChangeGroupsAnnouncementJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QString announcement() const;
    void setAnnouncement(const QString &topic);
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

Q_SIGNALS:
    void changeGroupsAnnouncement();

private:
    Q_DISABLE_COPY(ChangeGroupsAnnouncementJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mAnnouncement;
};
}
