/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT GetRoomsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetRoomsJob(QObject *parent = nullptr);
    ~GetRoomsJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

Q_SIGNALS:
    void getRoomsDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(GetRoomsJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    // TODO add updatedSince
};
}
