/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT SetTopicDmJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SetTopicDmJob(QObject *parent = nullptr);
    ~SetTopicDmJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString directUserId() const;
    void setDirectUserId(const QString &userId);

Q_SIGNALS:
    void setTopicDmDone();

private:
    Q_DISABLE_COPY(SetTopicDmJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mDirectUserId;
    QString mTopic;
};
}
