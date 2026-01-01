/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT UpdateMessageJob : public RestApiAbstractJob
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

    [[nodiscard]] QByteArray messageId() const;
    void setMessageId(const QByteArray &msgId);

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &roomId);

    [[nodiscard]] QString updatedText() const;
    void setUpdatedText(const QString &updatedText);

Q_SIGNALS:
    void updateMessageDone();
    void updateMessageFailed(const QString &str);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mRoomId;
    QByteArray mMessageId;
    QString mUpdatedText;
};
}
