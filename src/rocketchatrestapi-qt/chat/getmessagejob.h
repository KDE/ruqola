/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QString messageId() const;
    void setMessageId(const QString &messageId);

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

Q_SIGNALS:
    void getMessageDone(const QJsonObject &obj, const QString &messageId, const QString &roomId);

private:
    Q_DISABLE_COPY(GetMessageJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mMessageId;
    QString mRoomId;
};
}
