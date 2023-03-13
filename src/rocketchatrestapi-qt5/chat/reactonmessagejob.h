/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ReactOnMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ReactOnMessageJob(QObject *parent = nullptr);
    ~ReactOnMessageJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &msgId);

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString emoji() const;
    void setEmoji(const QString &emoji);

    void setShouldReact(bool added);
    Q_REQUIRED_RESULT bool shouldReact() const;

protected:
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &details) override;

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
