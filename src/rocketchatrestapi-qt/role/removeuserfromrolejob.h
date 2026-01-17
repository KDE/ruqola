/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] const QString &roleId() const;
    void setRoleId(const QString &newRoleId);

    [[nodiscard]] bool useRC80() const;
    void setUseRC80(bool newUseRC80);

Q_SIGNALS:
    void removeUsersFromRoleDone(const QJsonObject &replyObject);

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &details) override;

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QString mRoleName;
    QString mUsername;
    QString mRoleId;
    bool mUseRC80 = false;
};
}
