/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChangeChannelAnnouncementJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChangeChannelAnnouncementJob(QObject *parent = nullptr);
    ~ChangeChannelAnnouncementJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QString announcement() const;
    void setAnnouncement(const QString &topic);
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

Q_SIGNALS:
    void changeAnnouncementDone();

private:
    Q_DISABLE_COPY(ChangeChannelAnnouncementJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mAnnouncement;
    QString mRoomId;
};
}
