/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT SendMessageJob : public RestApiAbstractJob
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
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    SendMessageArguments mSendMessageArguments;
};
}
Q_DECLARE_METATYPE(RocketChatRestApi::SendMessageJob::SendMessageArguments)
Q_DECLARE_TYPEINFO(RocketChatRestApi::SendMessageJob::SendMessageArguments, Q_RELOCATABLE_TYPE);
