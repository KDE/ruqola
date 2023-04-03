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

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT const QDateTime &updatedSince() const;
    void setUpdatedSince(const QDateTime &newUpdatedSince);

Q_SIGNALS:
    void permissionListAllDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(PermissionsListAllJob)
    LIBROCKETCHATRESTAPI_QT5_NO_EXPORT void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    QDateTime mUpdatedSince;
};
}
