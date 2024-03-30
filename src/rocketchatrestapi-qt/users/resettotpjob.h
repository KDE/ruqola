/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ResetTOTPJob : public RestApiAbstractJob
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

    [[nodiscard]] const QByteArray &resetUserId() const;
    void setResetUserId(const QByteArray &newResetUserId);

Q_SIGNALS:
    void resetTOTPDone();

private:
    Q_DISABLE_COPY(ResetTOTPJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mResetUserId;
};
}
