/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT ChangeChannelAnnouncementJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChangeChannelAnnouncementJob(QObject *parent = nullptr);
    ~ChangeChannelAnnouncementJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QString announcement() const;
    void setAnnouncement(const QString &topic);
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

Q_SIGNALS:
    void changeAnnouncementDone();

private:
    Q_DISABLE_COPY(ChangeChannelAnnouncementJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mAnnouncement;
    QString mRoomId;
};
}
