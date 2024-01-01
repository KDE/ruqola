/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RemoveUserFromRoleJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit RemoveUserFromRoleJob(QObject *parent = nullptr);
    ~RemoveUserFromRoleJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] const QString &roleName() const;
    void setRoleName(const QString &newRoleName);

    [[nodiscard]] const QString &username() const;
    void setUsername(const QString &newUsername);

Q_SIGNALS:
    void removeUsersFromRoleDone(const QJsonObject &replyObject);

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &details) override;

private:
    Q_DISABLE_COPY(RemoveUserFromRoleJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoleName;
    QString mUsername;
};
}
