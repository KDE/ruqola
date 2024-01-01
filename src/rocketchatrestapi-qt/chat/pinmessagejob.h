/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT PinMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit PinMessageJob(QObject *parent = nullptr);
    ~PinMessageJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QString messageId() const;
    void setMessageId(const QString &messageId);

    [[nodiscard]] bool pinMessage() const;
    void setPinMessage(bool pinMessage);

    [[nodiscard]] QJsonDocument json() const;
Q_SIGNALS:
    void pinMessageDone();
    void unPinMessageDone();

private:
    Q_DISABLE_COPY(PinMessageJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mMessageId;
    bool mPinMessage = true;
};
}
