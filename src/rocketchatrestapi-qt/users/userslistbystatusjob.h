/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

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
    enum class Status : uint8_t {
        Unknown = 0,
        Deactivated = 1,
        Activated = 2,
    };
    Q_ENUM(Status)

    enum class StatusType : uint8_t {
        Unknown = 0,
        User = 1,
    };
    Q_ENUM(StatusType)

    enum class LoggedStatus : uint8_t {
        Unknown,
        Logged,
        NotLogged,
    };
    Q_ENUM(LoggedStatus)

    struct LIBROCKETCHATRESTAPI_QT_EXPORT UsersListByStatusJobInfo {
        Status status = Status::Unknown;
        StatusType type = StatusType::Unknown;
        QStringList roles;
        LoggedStatus hasLoggedIn = LoggedStatus::Unknown;
        QString searchName;
        [[nodiscard]] bool isValid() const;
        [[nodiscard]] QString statusToString() const;
        [[nodiscard]] QString typeToString() const;
        [[nodiscard]] QString loggedInToString() const;
    };
    explicit UsersListByStatusJob(QObject *parent = nullptr);
    ~UsersListByStatusJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] bool canStart() const override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] UsersListByStatusJobInfo usersListByStatusJobInfo() const;
    void setUsersListByStatusJobInfo(const UsersListByStatusJobInfo &newUsersListByStatusJobInfo);

    [[nodiscard]] bool hasQueryParameterSupport() const override;

Q_SIGNALS:
    void usersListByStatusDone(const QJsonObject &obj);

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    UsersListByStatusJobInfo mUsersListByStatusJobInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::UsersListByStatusJob::UsersListByStatusJobInfo, Q_RELOCATABLE_TYPE);
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::UsersListByStatusJob::UsersListByStatusJobInfo &t);
