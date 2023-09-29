/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT TranslateMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit TranslateMessageJob(QObject *parent = nullptr);
    ~TranslateMessageJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString messageId() const;
    void setMessageId(const QString &messageId);

    [[nodiscard]] QString targetLanguage() const;
    void setTargetLanguage(const QString &targetLanguage);

Q_SIGNALS:
    void translateMessageDone();

private:
    Q_DISABLE_COPY(TranslateMessageJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mMessageId;
    QString mTargetLanguage;
};
}
