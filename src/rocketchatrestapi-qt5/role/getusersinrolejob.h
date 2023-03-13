/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT GetUsersInRoleJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit GetUsersInRoleJob(QObject *parent = nullptr);
    ~GetUsersInRoleJob() override;

    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;

    Q_REQUIRED_RESULT bool start() override;

    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT bool hasQueryParameterSupport() const override;

    Q_REQUIRED_RESULT const QString &roleId() const;
    void setRoleId(const QString &newRoleId);

    Q_REQUIRED_RESULT bool canStart() const override;

Q_SIGNALS:
    void getUsersInRoleDone(const QJsonObject &obj);

private:
    Q_DISABLE_COPY(GetUsersInRoleJob)
    void onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    void initialUrlParameters(QUrlQuery &urlQuery) const;
    QString mRoleId;
};
}
