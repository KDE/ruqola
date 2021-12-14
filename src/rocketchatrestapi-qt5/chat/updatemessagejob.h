/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &msgId);

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    Q_REQUIRED_RESULT QString updatedText() const;
    void setUpdatedText(const QString &updatedText);

Q_SIGNALS:
    void updateMessageDone();

private:
    Q_DISABLE_COPY(UpdateMessageJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mRoomId;
    QString mMessageId;
    QString mUpdatedText;
};
}
