/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ResetE2EKeyJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ResetE2EKeyJob(QObject *parent = nullptr);
    ~ResetE2EKeyJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;
    [[nodiscard]] bool requireTwoFactorAuthentication() const override;

    [[nodiscard]] const QString &resetUserId() const;
    void setResetUserId(const QString &newResetUserId);

Q_SIGNALS:
    void resetE2EKeyDone();

private:
    Q_DISABLE_COPY(ResetE2EKeyJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mResetUserId;
};
}
