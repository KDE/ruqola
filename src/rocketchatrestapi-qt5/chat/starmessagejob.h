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
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT StarMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit StarMessageJob(QObject *parent = nullptr);
    ~StarMessageJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &messageId);

    Q_REQUIRED_RESULT QNetworkRequest request() const override;
    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT bool starMessage() const;
    void setStarMessage(bool starMessage);

    Q_REQUIRED_RESULT bool canStart() const override;

Q_SIGNALS:
    void messageStarred();

private:
    Q_DISABLE_COPY(StarMessageJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mMessageId;
    bool mStarMessage = true;
};
}
