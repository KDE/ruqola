/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT ReactOnMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ReactOnMessageJob(QObject *parent = nullptr);
    ~ReactOnMessageJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QString messageId() const;
    void setMessageId(const QString &msgId);

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString emoji() const;
    void setEmoji(const QString &emoji);

    void setShouldReact(bool added);
    [[nodiscard]] bool shouldReact() const;

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &details) override;

Q_SIGNALS:
    void reactOnMessageDone();

private:
    Q_DISABLE_COPY(ReactOnMessageJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mMessageId;
    QString mEmoji;
    bool mAddReact = true;
};
}
