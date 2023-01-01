/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "librocketchatrestapi-qt5_export.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT DeleteDmJob : public ChannelGroupBaseJob
{
    Q_OBJECT
public:
    explicit DeleteDmJob(QObject *parent = nullptr);
    ~DeleteDmJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

Q_SIGNALS:
    void deleteDirectMessagesDone(const QString &identifier);

private:
    Q_DISABLE_COPY(DeleteDmJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
};
}
