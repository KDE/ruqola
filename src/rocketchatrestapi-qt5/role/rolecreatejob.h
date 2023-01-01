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
        Q_REQUIRED_RESULT bool isValid() const
        {
            return !name.isEmpty() && !scope.isEmpty();
        }
    };
    explicit RoleCreateJob(QObject *parent = nullptr);
    ~RoleCreateJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;
    Q_REQUIRED_RESULT RoleCreateInfo createRoleInfo() const;
    void setCreateRoleInfo(const RoleCreateInfo &statusCreateInfo);

Q_SIGNALS:
    void createRoleDone();

private:
    Q_DISABLE_COPY(RoleCreateJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    RoleCreateInfo mRoleCreateInfo;
};
}
