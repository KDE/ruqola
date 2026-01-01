/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT ReportMessageJob : public RestApiAbstractJob
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

    [[nodiscard]] QByteArray messageId() const;
    void setMessageId(const QByteArray &messageId);

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString reportMessage() const;
    void setReportMessage(const QString &reportMessage);

Q_SIGNALS:
    void reportMessageDone();

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mMessageId;
    QString mReportMessage;
};
}
