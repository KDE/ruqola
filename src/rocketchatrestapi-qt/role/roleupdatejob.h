/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT RoleUpdateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT RoleUpdateInfo {
        QString name;
        QString description;
        QString scope;
        QString identifier;
        bool mandatory2fa = false;
        [[nodiscard]] bool isValid() const
        {
            return !name.isEmpty() && !scope.isEmpty() && !identifier.isEmpty();
        }
    };
    explicit RoleUpdateJob(QObject *parent = nullptr);
    ~RoleUpdateJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;
    [[nodiscard]] RoleUpdateInfo updateRoleInfo() const;
    void setUpdateRoleInfo(const RoleUpdateInfo &statusCreateInfo);

Q_SIGNALS:
    void updateRoleDone();

private:
    Q_DISABLE_COPY(RoleUpdateJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    RoleUpdateInfo mRoleCreateInfo;
};
}
