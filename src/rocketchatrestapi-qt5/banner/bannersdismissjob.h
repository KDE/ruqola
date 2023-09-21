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

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] const QString &bannerId() const;
    void setBannerId(const QString &newBannerId);

Q_SIGNALS:
    void dimissBannerDone();

private:
    Q_DISABLE_COPY(BannersDismissJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mBannerId;
};
}
