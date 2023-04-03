/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT ResetE2EKeyJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ResetE2EKeyJob(QObject *parent = nullptr);
    ~ResetE2EKeyJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;
    Q_REQUIRED_RESULT bool requireTwoFactorAuthentication() const override;

    Q_REQUIRED_RESULT const QString &resetUserId() const;
    void setResetUserId(const QString &newResetUserId);

Q_SIGNALS:
    void resetE2EKeyDone();

private:
    Q_DISABLE_COPY(ResetE2EKeyJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mResetUserId;
};
}
