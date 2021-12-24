/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT UnFollowMessageJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit UnFollowMessageJob(QObject *parent = nullptr);
    ~UnFollowMessageJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QString messageId() const;
    void setMessageId(const QString &msgId);

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

Q_SIGNALS:
    void unFollowMessageDone();

private:
    Q_DISABLE_COPY(UnFollowMessageJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mMessageId;
};
}
