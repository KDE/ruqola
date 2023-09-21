/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT ResetTOTPJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ResetTOTPJob(QObject *parent = nullptr);
    ~ResetTOTPJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;
    [[nodiscard]] bool requireTwoFactorAuthentication() const override;

    [[nodiscard]] const QString &resetUserId() const;
    void setResetUserId(const QString &newResetUserId);

Q_SIGNALS:
    void resetTOTPDone();

private:
    Q_DISABLE_COPY(ResetTOTPJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mResetUserId;
};
}
