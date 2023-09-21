/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT UpdateMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit UpdateMessageJob(QObject *parent = nullptr);
    ~UpdateMessageJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString messageId() const;
    void setMessageId(const QString &msgId);

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    [[nodiscard]] QString updatedText() const;
    void setUpdatedText(const QString &updatedText);

Q_SIGNALS:
    void updateMessageDone();
    void updateMessageFailed(const QString &str);

private:
    Q_DISABLE_COPY(UpdateMessageJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
    QString mMessageId;
    QString mUpdatedText;
};
}
