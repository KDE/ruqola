/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT TranslateMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TranslateMessageJob(QObject *parent = nullptr);
    ~TranslateMessageJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &messageId);

    Q_REQUIRED_RESULT QString targetLanguage() const;
    void setTargetLanguage(const QString &targetLanguage);

Q_SIGNALS:
    void translateMessageDone();

private:
    Q_DISABLE_COPY(TranslateMessageJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mMessageId;
    QString mTargetLanguage;
};
}
