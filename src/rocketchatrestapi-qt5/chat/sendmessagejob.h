/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT SendMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SendMessageJob(QObject *parent = nullptr);
    ~SendMessageJob() override;
    struct SendMessageArguments {
        QString messageId;
        QString roomId;
        QString threadMessageId;
        QString message;
    };

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] SendMessageArguments sendMessageArguments() const;
    void setSendMessageArguments(const SendMessageArguments &sendMessageArguments);

Q_SIGNALS:
    void sendMessageDone();

private:
    Q_DISABLE_COPY(SendMessageJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    SendMessageArguments mSendMessageArguments;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::SendMessageJob::SendMessageArguments)
Q_DECLARE_TYPEINFO(RocketChatRestApi::SendMessageJob::SendMessageArguments, Q_MOVABLE_TYPE);
