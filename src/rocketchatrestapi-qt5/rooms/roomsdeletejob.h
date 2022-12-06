/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT RoomsDeleteJob : public RestApiAbstractJob
{
    Q_OBJECT

    // Since rocketchat 0.64
public:
    explicit RoomsDeleteJob(QObject *parent = nullptr);
    ~RoomsDeleteJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

Q_SIGNALS:
    void roomDeleteDone();

private:
    Q_DISABLE_COPY(RoomsDeleteJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mRoomId;
};
}
