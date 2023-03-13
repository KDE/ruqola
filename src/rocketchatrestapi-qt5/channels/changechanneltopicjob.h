/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChangeChannelTopicJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChangeChannelTopicJob(QObject *parent = nullptr);
    ~ChangeChannelTopicJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QString topic() const;
    void setTopic(const QString &topic);
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

Q_SIGNALS:
    void changeTopicDone();

private:
    Q_DISABLE_COPY(ChangeChannelTopicJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mTopic;
    QString mRoomId;
};
}
