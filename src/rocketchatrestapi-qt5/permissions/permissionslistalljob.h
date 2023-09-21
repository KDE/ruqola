/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QDateTime>
#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT PermissionsListAllJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit PermissionsListAllJob(QObject *parent = nullptr);
    ~PermissionsListAllJob() override;

    [[nodiscard]] bool requireHttpAuthentication() const override;

    [[nodiscard]] bool start() override;

    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] const QDateTime &updatedSince() const;
    void setUpdatedSince(const QDateTime &newUpdatedSince);

Q_SIGNALS:
    void permissionListAllDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(PermissionsListAllJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QDateTime mUpdatedSince;
};
}
