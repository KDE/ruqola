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
    enum class Status {
        Unknown = 0,
        Desactivated = 1,
        Activated = 2,
    };
    Q_ENUM(Status)

    enum class StatusType {
        Unknown = 0,
        User = 1,
    };
    Q_ENUM(StatusType)

    struct LIBROCKETCHATRESTAPI_QT_EXPORT UsersListByStatusJobInfo {
        Status status = Status::Unknown;
        StatusType type = StatusType::Unknown;
        QStringList roles;
        bool hasLoggedIn = false;
        int count = 0;
        [[nodiscard]] bool isValid() const;
        [[nodiscard]] QString statusToString() const;
        [[nodiscard]] QString typeToString() const;
    };
    explicit UsersListByStatusJob(QObject *parent = nullptr);
    ~UsersListByStatusJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] UsersListByStatusJobInfo usersListByStatusJobInfo() const;
    void setUsersListByStatusJobInfo(const UsersListByStatusJobInfo &newUsersListByStatusJobInfo);

Q_SIGNALS:
    void usersListByStatusDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(UsersListByStatusJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    UsersListByStatusJobInfo mUsersListByStatusJobInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::UsersListByStatusJob::UsersListByStatusJobInfo, Q_RELOCATABLE_TYPE);
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::UsersListByStatusJob::UsersListByStatusJobInfo &t);
