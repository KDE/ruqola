/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
class QNetworkRequest;
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChannelGetAllUserMentionsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChannelGetAllUserMentionsJob(QObject *parent = nullptr);
    ~ChannelGetAllUserMentionsJob() override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void channelGetAllUserMentionsDone(const QJsonObject &obj, const QString &roomId);

private:
    Q_DISABLE_COPY(ChannelGetAllUserMentionsJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoomId;
};
}
