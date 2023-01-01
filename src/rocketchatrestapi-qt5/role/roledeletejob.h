/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT RoleDeleteJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit RoleDeleteJob(QObject *parent = nullptr);
    ~RoleDeleteJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT const QString &roleId() const;
    void setRoleId(const QString &newRoleId);

Q_SIGNALS:
    void deleteRoleDone();

private:
    Q_DISABLE_COPY(RoleDeleteJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    QString mRoleId;
};
}
