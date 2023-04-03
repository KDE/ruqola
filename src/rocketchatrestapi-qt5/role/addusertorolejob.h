/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT AddUserToRoleJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit AddUserToRoleJob(QObject *parent = nullptr);
    ~AddUserToRoleJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT const QString &roleName() const;
    void setRoleName(const QString &newRoleName);

    Q_REQUIRED_RESULT const QString &username() const;
    void setUsername(const QString &newUsername);

Q_SIGNALS:
    void addUsersToRoleDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(AddUserToRoleJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoleName;
    QString mUsername;
};
}
