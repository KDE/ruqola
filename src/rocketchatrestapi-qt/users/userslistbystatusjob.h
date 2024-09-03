/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT UsersListByStatusJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    enum Status {
        Unknown = 0,
        Desactivated = 1,
        Activated = 2,
    };
    struct LIBROCKETCHATRESTAPI_QT_EXPORT UsersListByStatusJobInfo {
        Status status = Unknown;
        bool hasLoggedIn = false;
        int count = 0;
    };
    explicit UsersListByStatusJob(QObject *parent = nullptr);
    ~UsersListByStatusJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

Q_SIGNALS:
    void usersPresenceDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(UsersListByStatusJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
};
}
