/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QMap>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT PermissionUpdateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit PermissionUpdateJob(QObject *parent = nullptr);
    ~PermissionUpdateJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT const QMap<QString, QStringList> &permissions() const;
    void setPermissions(const QMap<QString, QStringList> &newPermissions);

Q_SIGNALS:
    void permissionUpdateDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(PermissionUpdateJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    // Id, roles
    QMap<QString, QStringList> mPermissions;
};
}
