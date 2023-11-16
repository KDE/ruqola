/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT GetRoomsJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetRoomsJob(QObject *parent = nullptr);
    ~GetRoomsJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

Q_SIGNALS:
    void getRoomsDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(GetRoomsJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    // TODO add updatedSince
};
}
