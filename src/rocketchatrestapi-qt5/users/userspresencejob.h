/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT UsersPresenceJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // TODO userId or Username
    explicit UsersPresenceJob(QObject *parent = nullptr);
    ~UsersPresenceJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT bool canStart() const override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

Q_SIGNALS:
    void usersPresenceDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(UsersPresenceJob)
    void slotUsersPresenceFinished();
};
}
