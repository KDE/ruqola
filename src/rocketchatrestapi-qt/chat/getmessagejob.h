/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT GetMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetMessageJob(QObject *parent = nullptr);
    ~GetMessageJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QByteArray messageId() const;
    void setMessageId(const QByteArray &messageId);

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &roomId);

Q_SIGNALS:
    void getMessageDone(const QJsonObject &obj, const QByteArray &messageId, const QByteArray &roomId);

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &details) override;

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mMessageId;
    QByteArray mRoomId;
};
}
