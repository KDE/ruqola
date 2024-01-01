/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT DeleteMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit DeleteMessageJob(QObject *parent = nullptr);
    ~DeleteMessageJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QString messageId() const;
    void setMessageId(const QString &msgId);
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);
Q_SIGNALS:
    void deleteMessageDone();

protected:
    [[nodiscard]] QString jobName() const override;
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &details) override;

private:
    Q_DISABLE_COPY(DeleteMessageJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
    QString mMessageId;
};
}
