/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
class QNetworkRequest;
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT StarMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit StarMessageJob(QObject *parent = nullptr);
    ~StarMessageJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] QByteArray messageId() const;
    void setMessageId(const QByteArray &messageId);

    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] bool starMessage() const;
    void setStarMessage(bool starMessage);

    [[nodiscard]] bool canStart() const override;

Q_SIGNALS:
    void messageStarred();

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QByteArray mMessageId;
    bool mStarMessage = true;
};
}
