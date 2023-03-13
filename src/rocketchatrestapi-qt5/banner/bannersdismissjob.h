/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT BannersDismissJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit BannersDismissJob(QObject *parent = nullptr);
    ~BannersDismissJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT const QString &bannerId() const;
    void setBannerId(const QString &newBannerId);

Q_SIGNALS:
    void dimissBannerDone();

private:
    Q_DISABLE_COPY(BannersDismissJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mBannerId;
};
}
