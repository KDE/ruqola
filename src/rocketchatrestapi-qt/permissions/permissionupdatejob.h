/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

#include <QMap>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT PermissionUpdateJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit PermissionUpdateJob(QObject *parent = nullptr);
    ~PermissionUpdateJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] const QMap<QString, QStringList> &permissions() const;
    void setPermissions(const QMap<QString, QStringList> &newPermissions);

Q_SIGNALS:
    void permissionUpdateDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(PermissionUpdateJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    // Id, roles
    QMap<QString, QStringList> mPermissions;
};
}
