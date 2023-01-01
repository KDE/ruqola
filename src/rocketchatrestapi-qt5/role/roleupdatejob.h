/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT RoleUpdateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT RoleUpdateInfo {
        QString name;
        QString description;
        QString scope;
        QString identifier;
        bool mandatory2fa = false;
        Q_REQUIRED_RESULT bool isValid() const
        {
            return !name.isEmpty() && !scope.isEmpty() && !identifier.isEmpty();
        }
    };
    explicit RoleUpdateJob(QObject *parent = nullptr);
    ~RoleUpdateJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;
    Q_REQUIRED_RESULT RoleUpdateInfo updateRoleInfo() const;
    void setUpdateRoleInfo(const RoleUpdateInfo &statusCreateInfo);

Q_SIGNALS:
    void updateRoleDone();

private:
    Q_DISABLE_COPY(RoleUpdateJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    RoleUpdateInfo mRoleCreateInfo;
};
}
