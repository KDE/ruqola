/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT UsersPresenceJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // TODO userId or Username
    explicit UsersPresenceJob(QObject *parent = nullptr);
    ~UsersPresenceJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

Q_SIGNALS:
    void usersPresenceDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(UsersPresenceJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
