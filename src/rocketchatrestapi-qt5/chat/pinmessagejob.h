/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT PinMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit PinMessageJob(QObject *parent = nullptr);
    ~PinMessageJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &messageId);

    Q_REQUIRED_RESULT bool pinMessage() const;
    void setPinMessage(bool pinMessage);

    Q_REQUIRED_RESULT QJsonDocument json() const;
Q_SIGNALS:
    void pinMessageDone();
    void unPinMessageDone();

private:
    Q_DISABLE_COPY(PinMessageJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mMessageId;
    bool mPinMessage = true;
};
}
