/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT DeleteMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit DeleteMessageJob(QObject *parent = nullptr);
    ~DeleteMessageJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &msgId);
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);
Q_SIGNALS:
    void deleteMessageDone();

protected:
    Q_REQUIRED_RESULT QString jobName() const override;
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &details) override;

private:
    Q_DISABLE_COPY(DeleteMessageJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mRoomId;
    QString mMessageId;
};
}
