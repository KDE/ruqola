/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT FollowMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit FollowMessageJob(QObject *parent = nullptr);
    ~FollowMessageJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &msgId);

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

Q_SIGNALS:
    void followMessageDone();

private:
    Q_DISABLE_COPY(FollowMessageJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mMessageId;
};
}
