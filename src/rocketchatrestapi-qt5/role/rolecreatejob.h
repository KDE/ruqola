/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT RoleCreateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT RoleCreateInfo {
        QString name;
        QString description;
        QString scope;
        bool mandatory2fa = false;
        [[nodiscard]] bool isValid() const
        {
            return !name.isEmpty() && !scope.isEmpty();
        }
    };
    explicit RoleCreateJob(QObject *parent = nullptr);
    ~RoleCreateJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;
    [[nodiscard]] RoleCreateInfo createRoleInfo() const;
    void setCreateRoleInfo(const RoleCreateInfo &statusCreateInfo);

Q_SIGNALS:
    void createRoleDone();

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &detail) override;

private:
    Q_DISABLE_COPY(RoleCreateJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    RoleCreateInfo mRoleCreateInfo;
};
}
