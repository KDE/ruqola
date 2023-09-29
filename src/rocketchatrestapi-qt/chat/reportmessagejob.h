/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT ReportMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // since 0.64
    explicit ReportMessageJob(QObject *parent = nullptr);
    ~ReportMessageJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QString messageId() const;
    void setMessageId(const QString &messageId);

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString reportMessage() const;
    void setReportMessage(const QString &reportMessage);

Q_SIGNALS:
    void reportMessageDone();

private:
    Q_DISABLE_COPY(ReportMessageJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mMessageId;
    QString mReportMessage;
};
}
