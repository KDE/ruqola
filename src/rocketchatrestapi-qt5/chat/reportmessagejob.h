/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ReportMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // since 0.64
    explicit ReportMessageJob(QObject *parent = nullptr);
    ~ReportMessageJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &messageId);

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString reportMessage() const;
    void setReportMessage(const QString &reportMessage);

Q_SIGNALS:
    void reportMessageDone();

private:
    Q_DISABLE_COPY(ReportMessageJob)
    void slotReportMessage();
    QString mMessageId;
    QString mReportMessage;
};
}
