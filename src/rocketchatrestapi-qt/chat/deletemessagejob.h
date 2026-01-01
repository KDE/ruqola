/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT DeleteMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit DeleteMessageJob(QObject *parent = nullptr);
    ~DeleteMessageJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QByteArray messageId() const;
    void setMessageId(const QByteArray &msgId);
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &roomId);
Q_SIGNALS:
    void deleteMessageDone();

protected:
    [[nodiscard]] QString jobName() const override;
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &details) override;

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mRoomId;
    QByteArray mMessageId;
};
}
