/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT ChangeChannelNameJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit ChangeChannelNameJob(QObject *parent = nullptr);
    ~ChangeChannelNameJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &t);
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

Q_SIGNALS:
    void changeNameDone();

private:
    Q_DISABLE_COPY(ChangeChannelNameJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mName;
    QString mRoomId;
};
}
