/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
class QNetworkRequest;
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT StarMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit StarMessageJob(QObject *parent = nullptr);
    ~StarMessageJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] QString messageId() const;
    void setMessageId(const QString &messageId);

    [[nodiscard]] QNetworkRequest request() const override;
    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] bool starMessage() const;
    void setStarMessage(bool starMessage);

    [[nodiscard]] bool canStart() const override;

Q_SIGNALS:
    void messageStarred();

private:
    Q_DISABLE_COPY(StarMessageJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mMessageId;
    bool mStarMessage = true;
};
}
